/*
 * ihm.c
 *
 *  Created on: 20/06/2017
 *      Author: Fl�vio Soares
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "XF1.h"

#include "lcd.h"
#include "gps.h"
#include "telemetria.h"
#include "serial.h"
#include "clock.h"
#include "ihm.h"

static screen active_screen;
static char line_lcd[25];

/* Tempo do splash em segundos */
static int time_splash;


/* Task IHM*/
static const char*			IHM_TASK_NAME =			"tk_ihm";
#define 					IHM_TASK_PRIORITY		(tskIDLE_PRIORITY+3)
#define						IHM_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE+100)
static TaskHandle_t			xHandleIHMTask;
static EventGroupHandle_t	ihm_events;

/* Timer */
TimerHandle_t			 	xTimerUpdateStat;
static const char*			UPDATE_TIME_NAME =		"tm_stat";
#define						UPDATE_TIME_STAT		pdMS_TO_TICKS( 200 )

static void ihm_notify_screen_clock(void);

static portTASK_FUNCTION(run_ihm, pvParameters) {

	while(1) {

		ihm_task();
	}

	vTaskDelete(xHandleIHMTask);
}
//-------------------------------------------------------------------------

void updateTimer_Stat( TimerHandle_t xTimer ){

	ihm_notify_screen_stat();
}
//-------------------------------------------------------------------------

static void createTask(void){

	if (xTaskCreate(
		run_ihm,
		IHM_TASK_NAME,
		IHM_TASK_STACK_SIZE,
		(void*)NULL,
		IHM_TASK_PRIORITY,
		&xHandleIHMTask
	) != pdPASS) {

		while(1) {};
	}

	xTimerUpdateStat = xTimerCreate (
			UPDATE_TIME_NAME,
			UPDATE_TIME_STAT,
			pdTRUE,
			( void * ) 0,
			updateTimer_Stat
    );

    if( xTimerStart( xTimerUpdateStat, 0 ) != pdPASS ){
    	//Problema no start do timer
    }
}
//--------------------------------------------------------------------------------

void ihm_task(void) {

	EventBits_t uxBits	= xEventGroupWaitBits(
									ihm_events,
									BIT_UPDATE_LCD_CLOCK	|
									BIT_UPDATE_LCD_XYZ		|
									BIT_UPDATE_LCD_STAT_COM |
									BIT_UPDATE_LCD_STAT_GPS |
									BIT_UPDATE_LCD_GPS		|
									BIT_UPDATE_LCD_TANK,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY );


	//Hook de processamento dos eventos

	if(uxBits & BIT_UPDATE_LCD_CLOCK){

		printClock();
	}

	if(uxBits & BIT_UPDATE_LCD_XYZ){
		printAccelerometer();
	}

	if(uxBits & BIT_UPDATE_LCD_STAT_COM){
		printStatCom();
	}

	if(uxBits & BIT_UPDATE_LCD_STAT_GPS){
		printStatGPS();
	}

	if(uxBits & BIT_UPDATE_LCD_TANK){
		printTank();
	}

	if(uxBits & BIT_UPDATE_LCD_GPS){
		printGPS();
	}
}
//-----------------------------------------------------------------------------------------

void printGPS(void){

	TO_STRING(line_lcd,"Lat:%12.7f",	telemetria.GPS.Lat);
	printLCD(1,1,line_lcd);

	TO_STRING(line_lcd,"Lng:%12.7f",	telemetria.GPS.Lng);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printTank(void){

	TO_STRING(line_lcd,"Level: %05d    ",	telemetria.Tank.Level);
	printLCD(1,1,line_lcd);

	TO_STRING(line_lcd,"Lock:  %d         ",	telemetria.Tank.Lock);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printAccelerometer(void){

	TO_STRING(line_lcd,"X:%c%4.2f Y:%c%4.2f   ",	telemetria.Accelerometer.x_g>=0?'+':'-',fabs(telemetria.Accelerometer.x_g),
														telemetria.Accelerometer.y_g>=0?'+':'-',fabs(telemetria.Accelerometer.y_g)
													);
	printLCD(1,1,line_lcd);

	TO_STRING(line_lcd,"Z:%c%4.2f         	  ",	telemetria.Accelerometer.z_g>=0?'+':'-',fabs(telemetria.Accelerometer.z_g)
													);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printStatCom(void){

	TO_STRING(line_lcd,"RM:%03dP:%03dC:%03d",bufferRx.max_count,bufferRx.index_producer,bufferRx.index_consumer);
	printLCD(1,1,line_lcd);

	TO_STRING(line_lcd,"TM:%03dP:%03dC:%03d",bufferTx.max_count,bufferTx.index_producer,bufferTx.index_consumer);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printStatGPS(void){

	TO_STRING(line_lcd,"RX: M:%03d        ",bufferRxNMEA.max_count);
	printLCD(1,1,line_lcd);

	TO_STRING(line_lcd,"RX: C:%03d P:%03d ",bufferRxNMEA.index_consumer,bufferRxNMEA.index_producer);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printSplash(void){

	printLCD(1,1,"**GOODSTRACKER**");
	printLCD(2,1,"    VER. 0.3    ");
}
//-----------------------------------------------------------------------------------------

void printClock(void){

	LDD_RTC_TTime	time;

	switch(statuc_clock){

		case CLOCK_INIT:
			TO_STRING(line_lcd,"    CLKINIT    ");

			break;
		case CLOCK_STARTED:
			TO_STRING(line_lcd,"    CLKSTART    ");
			break;
		case CLOCK_UPDATE:
			TO_STRING(line_lcd,"     CLKUPD     ");
			break;
		case CLOCK_ADJUSTED:
			if(getLocalClock(&time)){
				TO_STRING(line_lcd," %02d.%02d %02d:%02d:%02d \n",time.Day,time.Month,time.Hour, time.Minute, time.Second);

			}else{
				statuc_clock = CLOCK_ERROR;
			}

			break;

		case CLOCK_ERROR:
			TO_STRING(line_lcd,"     ERROR     ");
			break;

	}

	printLCD(1,1,"**GOODSTRACKER**");
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printLCD(int linha,int col,char* str){

	LCDGotoXY(linha,col);
	LCDWriteString(str);
}
//-----------------------------------------------------------------------------------------

static void ihm_notify_screen_clock(void){

	BaseType_t xHigherPriorityTaskWoken, xResult;

    xHigherPriorityTaskWoken	= pdFALSE;
    xResult						= pdFAIL;

	if(active_screen == SCREEN_CLOCK){

		xResult = xEventGroupSetBitsFromISR(ihm_events, BIT_UPDATE_LCD_CLOCK , &xHigherPriorityTaskWoken);
	}

	if (xResult != pdFAIL){

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
//-----------------------------------------------------------------------------------------

void ihm_notify_screen_stat(void){

	if(active_screen == SCREEN_STAT_COM){

		xEventGroupSetBits(ihm_events, BIT_UPDATE_LCD_STAT_COM);
	}else if(active_screen == SCREEN_STAT_GPS){

		xEventGroupSetBits(ihm_events, BIT_UPDATE_LCD_STAT_GPS);
	}
}
//-----------------------------------------------------------------------------------------

void ihm_notify_screen_tlm(void){

	EventBits_t uxBitsToSet = 0;

	if(active_screen == SCREEN_ACCE){

		xEventGroupSetBits(ihm_events, BIT_UPDATE_LCD_XYZ);

	} else if(active_screen == SCREEN_TANK){

		xEventGroupSetBits(ihm_events, BIT_UPDATE_LCD_TANK);

	} else if(active_screen == SCREEN_GPS){

		xEventGroupSetBits(ihm_events, BIT_UPDATE_LCD_GPS);
	}
}
//-----------------------------------------------------------------------------------------

/*
 * Gera o evento para atualizacao periodica do relogio
 */
void ihm_handle_update(void){

	if(time_splash>0){

		// Detecta o momento da transição para o zero
		if(--time_splash<=0){

			ihm_set_active_screen(SCREEN_CLOCK);
		}

	}else{

		ihm_notify_screen_clock();
	}
}
//-----------------------------------------------------------------------------------------

inline void ihm_set_active_screen(screen s){

	active_screen = s;
}
//-----------------------------------------------------------------------------------------

/*
 *
 * Inicializa LCD
 *
 */
void ihm_init(void) {

	ihm_events	= xEventGroupCreate();

	LCDInit();

	time_splash		= TIME_SPLASH;
	active_screen	= SCREEN_SPLASH;

	// Habilitar o clock dos ports que serão utilizados (PORTD).
	//SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// Seleciona a função de GPIO
	//PORTD_PCR7 = PORT_PCR_MUX(1);

	// Configura como entrada
	//GPIOD_PDDR &= KEY_INPUT;

	printSplash();

	createTask();
}
//-----------------------------------------------------------------------------------------

void readKey(void){

	if(active_screen< (NUM_OF_SCREEN-1)){

		active_screen++;

	}else{

		ihm_set_active_screen(SCREEN_CLOCK);
	}
}
//-----------------------------------------------------------------------------------------

void ihm_deInit(void) {

}
//-----------------------------------------------------------------------------------------
