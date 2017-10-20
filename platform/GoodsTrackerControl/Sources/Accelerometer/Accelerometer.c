/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>
#include "MMA8451.h"

#include "application.h"
#include "Accelerometer.h"


/* Task */
static const char*		ACCE_TASK_NAME =		"tk_accelerometer";
#define 				ACCE_TASK_PRIORITY		(tskIDLE_PRIORITY)
#define					ACCE_TASK_STACK_SIZE	(configMINIMAL_STACK_SIZE)
static const TickType_t ACCE_TASK_DELAY	= 		(200 / portTICK_PERIOD_MS);
QueueHandle_t			xQueueAcce;
TaskHandle_t			xHandleAccelTask;

Accelerometer	acceInfo;


static portTASK_FUNCTION(run_accel, pvParameters) {

	MMA845x_init();

	while(1) {

		if(MMA845x_getXYZ(&acceInfo)){

			if(xQueueSendToBack( xQueueAcce ,  &acceInfo, ( TickType_t ) 1 ) ){

				xTaskNotify(xHandleAppTask, BIT_UPDATE_ACCE , eSetBits );
			}
		}

		vTaskDelay(ACCE_TASK_DELAY);
	}

	MMA845x_deInit();

	vTaskDelete(xHandleAccelTask);
}
//------------------------------------------------------------------------

static void createTask(void){

	if (xTaskCreate(
			run_accel,
			ACCE_TASK_NAME,
			ACCE_TASK_STACK_SIZE,
			(void*)NULL,
			ACCE_TASK_PRIORITY,
			&xHandleAccelTask
	) != pdPASS) {

		while (1) {};
	}
}
//------------------------------------------------------------------------

void accelerometer_init(void){

	clearAccelerometer(&acceInfo);

	xQueueAcce	= xQueueCreate( 1, sizeof( Accelerometer ));

	createTask();
}
//------------------------------------------------------------------------

void deInitAccelerometer(void){

	MMA845x_deInit();
}
//------------------------------------------------------------------------
