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

#include "lcd.h"
#include "XF1.h"

#include "lcd.h"
#include "gps.h"
#include "application.h"
#include "serial.h"
#include "clock.h"
#include "ihm.h"

#define KEY        	(1<<7)
#define KEY_INPUT	(~KEY)

static screen active_screen;
static char line_lcd[20];

static int time_splash	= 3;		//3 segundos

TaskHandle_t	xHandleIHMTask;

static EventGroupHandle_t	ihm_events;

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

	XF1_xsprintf(line_lcd,"Lat:%12.7f",	telemetria.GPS.Lat);
	printLCD(1,1,line_lcd);

	XF1_xsprintf(line_lcd,"Lng:%12.7f",	telemetria.GPS.Lng);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printTank(void){

	XF1_xsprintf(line_lcd,"Level: %05d    ",	telemetria.Tank.Level);
	printLCD(1,1,line_lcd);

	XF1_xsprintf(line_lcd,"Lock:  %d         ",	telemetria.Tank.Lock);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printAccelerometer(void){

	XF1_xsprintf(line_lcd,"X:%c%4.2f Y:%c%4.2f   ",	telemetria.Accelerometer.x_g>=0?'+':'-',fabs(telemetria.Accelerometer.x_g),
														telemetria.Accelerometer.y_g>=0?'+':'-',fabs(telemetria.Accelerometer.y_g)
													);
	printLCD(1,1,line_lcd);

	XF1_xsprintf(line_lcd,"Z:%c%4.2f         	  ",	telemetria.Accelerometer.z_g>=0?'+':'-',fabs(telemetria.Accelerometer.z_g)
													);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printStatCom(void){

	XF1_xsprintf(line_lcd,"RM:%03dP:%03dC:%03d",bufferRx.max_count,bufferRx.index_producer,bufferRx.index_consumer);
	printLCD(1,1,line_lcd);

	XF1_xsprintf(line_lcd,"TM:%03dP:%03dC:%03d",bufferTx.max_count,bufferTx.index_producer,bufferTx.index_consumer);
	printLCD(2,1,line_lcd);
}
//-----------------------------------------------------------------------------------------

void printStatGPS(void){

	XF1_xsprintf(line_lcd,"RX: M:%03d        ",bufferRxNMEA.max_count);
	printLCD(1,1,line_lcd);

	XF1_xsprintf(line_lcd,"RX: C:%03d P:%03d ",bufferRxNMEA.index_consumer,bufferRxNMEA.index_producer);
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
			XF1_xsprintf(line_lcd,"    CLKINIT    ");

			break;
		case CLOCK_STARTED:
			XF1_xsprintf(line_lcd,"    CLKSTART    ");
			break;
		case CLOCK_UPDATE:
			XF1_xsprintf(line_lcd,"     CLKUPD     ");
			break;
		case CLOCK_ADJUSTED:
			if(getLocalClock(&time)){
				XF1_xsprintf(line_lcd," %02d.%02d %02d:%02d:%02d \n",time.Day,time.Month,time.Hour, time.Minute, time.Second);

			}else{
				statuc_clock = CLOCK_ERROR;
			}

			break;

		case CLOCK_ERROR:
			XF1_xsprintf(line_lcd,"     ERROR     ");
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

void ihm_event_notify(EventBits_t uxBitsToSet){

	bool flag = false;

	if((active_screen ==SCREEN_CLOCK) && (uxBitsToSet & BIT_UPDATE_LCD_CLOCK)){

		flag = true;
	}

	if((active_screen ==SCREEN_ACCE) && (uxBitsToSet & BIT_UPDATE_LCD_XYZ)){

		flag = true;
	}

	if((active_screen ==SCREEN_STAT_COM) && (uxBitsToSet & BIT_UPDATE_LCD_STAT_COM)){

		flag = true;
	}

	if((active_screen ==SCREEN_STAT_GPS) && (uxBitsToSet & BIT_UPDATE_LCD_STAT_GPS)){

		flag = true;
	}

	if((active_screen ==SCREEN_GPS) && (uxBitsToSet & BIT_UPDATE_LCD_GPS)){

		flag = true;
	}


	if((active_screen ==SCREEN_TANK) && (uxBitsToSet & BIT_UPDATE_LCD_TANK)){

		flag = true;
	}

	if(flag){

		xEventGroupSetBits(ihm_events, uxBitsToSet);
	}
}
//-----------------------------------------------------------------------------------------

void ihm_notify_screen_stat(void){

	EventBits_t uxBitsToSet = 0;

	if(active_screen == SCREEN_STAT_COM){

		uxBitsToSet = BIT_UPDATE_LCD_STAT_COM;

	}else if(active_screen == SCREEN_STAT_GPS){

		uxBitsToSet = BIT_UPDATE_LCD_STAT_GPS;
	}

	ihm_event_notify(uxBitsToSet);
}
//-----------------------------------------------------------------------------------------

void ihm_notify_screen_tlm(void){

	EventBits_t uxBitsToSet = 0;

	if(active_screen == SCREEN_ACCE){

		uxBitsToSet = BIT_UPDATE_LCD_XYZ;

	} else if(active_screen == SCREEN_TANK){

		uxBitsToSet = BIT_UPDATE_LCD_TANK;

	} else if(active_screen == SCREEN_GPS){

		uxBitsToSet = BIT_UPDATE_LCD_GPS;
	}

	ihm_event_notify(uxBitsToSet);
}
//-----------------------------------------------------------------------------------------

void ihm_handle_update(void){

	if(time_splash>0){

		// Detecta o momento da transição para o zero
		if(--time_splash<=0){

			ihm_set_active_screen(SCREEN_ACCE);
		}

	}else{

		//TODO verificar a função correta quando chamado dentro de uma interrupção
		ihm_event_notify(BIT_UPDATE_LCD_CLOCK);
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

	active_screen = SCREEN_SPLASH;

	// Habilitar o clock dos ports que serão utilizados (PORTD).
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// Seleciona a função de GPIO
	PORTD_PCR7 = PORT_PCR_MUX(1);

	// Configura como entrada
	GPIOD_PDDR &= KEY_INPUT;

	printSplash();
}
//-----------------------------------------------------------------------------------------

void readKey(void){

	if(!(GPIOD_PDIR & KEY)){

		if(active_screen< (NUM_OF_SCREEN-1)){

			active_screen++;

		}else{

			ihm_set_active_screen(SCREEN_ACCE);
		}
	}
}
//-----------------------------------------------------------------------------------------

void ihm_deInit(void) {

}
//-----------------------------------------------------------------------------------------
