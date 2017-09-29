/*
 * tank.c
 *
 *  Created on: 22 de set de 2017
 *      Author: jefferson
 */

#include "consumer.h"
#include "tank.h"

static const TickType_t	xTaskDelay	= (1000 / portTICK_PERIOD_MS);

// Handles Tasks
TaskHandle_t 	xHandleTankTask;
QueueHandle_t	xQueueTank;

static Tank		tank;

volatile	bool AD_finished = false;

void tank_task(void){

	while(1){

		if(tank_getdata()){

			tank_sendResultQueue();
		}

        vTaskDelay(xTaskDelay);
	}
}
//------------------------------------------------------------------------

void tank_sendResultQueue(void){

	if(xQueueSendToBack( xQueueTank ,  &tank, ( TickType_t ) 1 ) ){

		xEventGroupSetBits(consumer_event, BIT_UPDATE_AD);
	}
}
//------------------------------------------------------------------------

void tank_init(void){

	xQueueTank		= xQueueCreate( 5, sizeof( Tank ));

	xTaskCreate(
		tank_task, /* pointer to the task */
		"tank_task", /* task name for kernel awareness debugging */
		1024, /* task stack size */
		(void*)NULL, /* optional task startup argument */
		5, /* initial priority */
		&xHandleTankTask /* optional task handle to create */
	);
}
//------------------------------------------------------------------------

bool tank_getdata(void){

	tank_random_data();

	return true;
}
//------------------------------------------------------------------------

void tank_random_data(void){

	tank.Level= 900+(rand() % 100);
	tank.Lock = 1;
}
//------------------------------------------------------------------------

void lock(void){

}
//------------------------------------------------------------------------

void unLock(void){

}
//------------------------------------------------------------------------
