/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */


#include "level_sensor.h"
#include "Telemetria.h"
#include "Tank.h"

/* Task Tank */
static const char*			TANK_TASK_NAME =			"tk_tank";
#define						TANK_NUM_MSG				1
#define 					TANK_TASK_PRIORITY			(tskIDLE_PRIORITY+2)
#define						TANK_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE-20)
QueueHandle_t				xQueueTank;
TaskHandle_t				xHandleDataTask;

/* Timer */
TimerHandle_t			 	xTimerTank;
static const char*			UPDATE_TIME_NAME =		"tm_tank";
#define						UPDATE_TIME_TANK		pdMS_TO_TICKS( 200 )

static	Tank				tank;


/**
 * Task de gerenciamento do tank
 *
 */
static portTASK_FUNCTION(run_data, pvParameters) {

	while(1) {

		EventBits_t uxBits	= xEventGroupWaitBits(tlm_events, BIT_AD_VALUE,pdTRUE,pdFALSE, portMAX_DELAY);

		if(uxBits & BIT_AD_VALUE){

			if(readValues(&tank.Level)){

				if(xQueueSendToBack( xQueueTank ,  &tank, ( TickType_t ) 1 ) == pdPASS){

					tlm_notify_tank();
				}
			}
		}
	}

	vTaskDelete(xHandleDataTask);
}
//---------------------------------------------------------------------------

void lock(void){

	LED_R_On();
	LED_G_Off();
}
//------------------------------------------------------------------------

void unLock(void){

	LED_G_On();
	LED_R_Off();
}
//------------------------------------------------------------------------

static void time_tank( TimerHandle_t xTimer ){

	start_measure();
}
//------------------------------------------------------------------------

static void createTask(void){

	if (xTaskCreate(
		run_data,
		TANK_TASK_NAME ,
		TANK_TASK_STACK_SIZE,
		(void*)NULL,
		TANK_TASK_PRIORITY,
		&xHandleDataTask
	) != pdPASS) {

	}

	xTimerTank = xTimerCreate (
			UPDATE_TIME_NAME,
			UPDATE_TIME_TANK,
			pdTRUE,
			( void * ) 0,
			time_tank
    );

    if( xTimerStart( xTimerTank, 0 ) != pdPASS ){
    	//Problema no start do timer
    }
}
//------------------------------------------------------------------------

void tank_init(void){

	level_sensor_init();

	xQueueTank		= xQueueCreate( TANK_NUM_MSG, sizeof( Tank ));

	createTask();
}
//-----------------------------------------------------------------------------
