/*
 * consumer.c
 *
 *  Created on: 22 de set de 2017
 *      Author: jefferson
 */

#include "communication.h"
#include "consumer.h"
#include "Telemetria.h"
#include "consumer.h"

// Task Delay
static const TickType_t	xConsummerDelay	= (1000 / portTICK_PERIOD_MS);

TaskHandle_t		xHandleConsumer;

EventGroupHandle_t	consumer_event;

void consumer_task(void){

	while(1){

		EventBits_t uxBits = xEventGroupWaitBits(
									consumer_event,
									BIT_UPDATE_GPS | BIT_UPDATE_ACCE | BIT_UPDATE_AD,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY );
		updateTLM(uxBits);

		//publishTLM(TRACKER_ADDRESS,&telemetria);

        vTaskDelay(xConsummerDelay);
	}
}
//-----------------------------------------------------------------------------

void consumer_init(void){

	consumer_event = xEventGroupCreate();

	xTaskCreate(
		consumer_task,
		"consumer_task",
		4096,
		(void*)NULL,
		4,
		&xHandleConsumer
	);
}
//-----------------------------------------------------------------------------
