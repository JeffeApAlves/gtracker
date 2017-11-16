/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>

#include "MMA8451.h"
#include "Telemetria.h"
#include "Accelerometer.h"


/* Task */
static const char*		ACCE_TASK_NAME =		"tk_acce";
#define					ACCE_NUM_MSG			1
#define 				ACCE_TASK_PRIORITY		(tskIDLE_PRIORITY+2)
#define					ACCE_TASK_STACK_SIZE	(configMINIMAL_STACK_SIZE-10)
static const TickType_t ACCE_TASK_DELAY	= 		(200 / portTICK_PERIOD_MS);
QueueHandle_t			xQueueAcce;
static TaskHandle_t		xHandleAccelTask;

static Accelerometer	accelerometer;

static portTASK_FUNCTION(run_accel, pvParameters) {

	if(MMA845x_init()){

		while(1) {

			if(MMA845x_getXYZ(&accelerometer)){

				if(xQueueSendToBack( xQueueAcce ,  &accelerometer, ( TickType_t ) 1 ) ){

					tlm_notify_accelerometer();
				}
			}

			vTaskDelay(ACCE_TASK_DELAY);
		}

		MMA845x_deInit();
	}

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

	}
}
//------------------------------------------------------------------------

void accelerometer_init(void){

	clearAccelerometer(&accelerometer);

	xQueueAcce	= xQueueCreate( ACCE_NUM_MSG , sizeof( Accelerometer ));

	createTask();
}
//------------------------------------------------------------------------

void deInitAccelerometer(void){

	MMA845x_deInit();
}
//------------------------------------------------------------------------
