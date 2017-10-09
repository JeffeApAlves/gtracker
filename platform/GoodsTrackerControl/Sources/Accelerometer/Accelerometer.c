/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "MMA8451.h"

#include "application.h"
#include "Accelerometer.h"

static const char* name_task = "task_accelerometer";

static const TickType_t xAcceDelay		= (200 / portTICK_PERIOD_MS);

Accelerometer	acceInfo;

QueueHandle_t	xQueueAcc;

TaskHandle_t xHandleAccelTask;

static void accelerometer_task(void) {

	if(MMA845x_getXYZ(&acceInfo)){

		if(xQueueSendToBack( xQueueAcc ,  &acceInfo, ( TickType_t ) 1 ) ){

			xTaskNotify(xHandleAppTask, BIT_UPDATE_ACCE , eSetBits );
		}
	}
}
//------------------------------------------------------------------------

static portTASK_FUNCTION(run_accel, pvParameters) {

	while(1) {

		accelerometer_task();

		vTaskDelay(xAcceDelay);
	}

	vTaskDelete(xHandleAccelTask);
}
//------------------------------------------------------------------------

static void createTask(void){

	if (FRTOS1_xTaskCreate(
			run_accel,
			name_task,
			configMINIMAL_STACK_SIZE + 0,
			(void*)NULL,
			tskIDLE_PRIORITY + 0,
			&xHandleAccelTask
	) != pdPASS) {

		for (;;) {};
	}
}
//------------------------------------------------------------------------

void accelerometer_init(void){

	createTask();

	xQueueAcc		= xQueueCreate( 1, sizeof( Accelerometer ));

	clearAccelerometer(&acceInfo);
	MMA845x_init();
}
//------------------------------------------------------------------------

void deInitAccelerometer(void){

	MMA845x_deInit();
}
//------------------------------------------------------------------------
