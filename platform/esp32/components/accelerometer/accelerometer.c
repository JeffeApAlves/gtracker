/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */
#include <stdio.h>


#include "communication.h"
#include "consumer.h"
#include "accelerometer.h"

Accelerometer	acceInfo;

const TickType_t	xAccelerometerDelay	= (500 / portTICK_PERIOD_MS);

// Handles Tasks
TaskHandle_t	xHandleAccelTask = NULL;

// Handles das Queues
QueueHandle_t	xQueueAcc;

void accelerometer_task(void) {

	while(1){

		if(accelerometer_getdata()){

			accelerometer_sendResultQueue();
		}

        vTaskDelay(xAccelerometerDelay);
	}
}
//------------------------------------------------------------------------

void accelerometer_sendResultQueue(void){

	if(xQueueSendToBack( xQueueAcc ,  &acceInfo, ( TickType_t ) 1 ) ){

		//xTaskNotify( xHandleConsumer , BIT_UPDATE_ACCE , eSetBits );
		xEventGroupSetBits(consumer_event, BIT_UPDATE_ACCE);
	}
}
//------------------------------------------------------------------------

bool accelerometer_getdata(void){

	accelerometer_random_data();

	return true;
}
//------------------------------------------------------------------------

void accelerometer_init(void){

	clearAccelerometer(&acceInfo);

	xQueueAcc		= xQueueCreate( 5, sizeof( Accelerometer ));

	xTaskCreate(
		accelerometer_task, /* pointer to the task */
		"accelerometer_task", /* task name for kernel awareness debugging */
		1024, /* task stack size */
		(void*)NULL, /* optional task startup argument */
		5, /* initial priority */
		&xHandleAccelTask /* optional task handle to create */
	);
}
//------------------------------------------------------------------------

void accelerometer_deInit(void){

}
//------------------------------------------------------------------------

void accelerometer_random_data(void){

	acceInfo.x	 = 1500 + rand() % 100;
	acceInfo.y	 = 1000 + rand() % 100;
	acceInfo.z	 = 2000 + rand() % 100;

	acceInfo.x_g = 0.5 + ((rand() % (20*TRACKER_ADDRESS))/100.0);
	acceInfo.y_g = 0.0 + ((rand() % (20*TRACKER_ADDRESS))/100.0);
	acceInfo.z_g = 1.0 + ((rand() % (20*TRACKER_ADDRESS))/100.0);
}
//------------------------------------------------------------------------------------
