/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "LED_B.h"
#include "LED_G.h"
#include "LED_R.h"

#include "FRTOS1.h"
#include "application.h"
#include "Tank.h"


/* Task Tank */
static const char*			TANK_TASK_NAME =			"task_tank";
#define 					TANK_TASK_PRIORITY			(tskIDLE_PRIORITY+3)
#define						TANK_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE)
static EventGroupHandle_t	TANK_events;
static const TickType_t		TANK_TASK_DELAY	= 			(200 / portTICK_PERIOD_MS);
QueueHandle_t				xQueueTank;
TaskHandle_t				xHandleDataTask;


static uint16_t	ADValues[AD1_CHANNEL_COUNT];
static Tank		tank;
bool AD_finished;

/**
 * Task de gerenciamento do tank
 *
 */
static portTASK_FUNCTION(run_data, pvParameters) {

	while(1) {

		tank_task();

		vTaskDelay(TANK_TASK_DELAY);
	}

	vTaskDelete(xHandleDataTask);
}
//---------------------------------------------------------------------------

/**
 * Leitura do tanque
 *
 */
void tank_task(void){

	AD_finished = false;

	if(AD1_Measure(true)==ERR_OK){

		while (!AD_finished) {}

		if(AD1_GetValue16(&ADValues[0])==ERR_OK){

			tank.Level = ADValues[0];

		    if(xQueueSendToBack( xQueueTank ,  &tank, ( TickType_t ) 1 ) ){

		    	xTaskNotify( xHandleAppTask, BIT_UPDATE_AD , eSetBits );
		    }
		}
	}
}
//------------------------------------------------------------------------

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

static void createTask(void){

	if (FRTOS1_xTaskCreate(
		run_data,
		TANK_TASK_NAME ,
		TANK_TASK_STACK_SIZE,
		(void*)NULL,
		TANK_TASK_PRIORITY,
		&xHandleDataTask
	) != pdPASS) {

		while(1) {};
	}
}
//------------------------------------------------------------------------

void tank_init(void){

	AD_finished		= false;

	xQueueTank		= xQueueCreate( 1, sizeof( Tank ));

	createTask();
}
//-----------------------------------------------------------------------------
