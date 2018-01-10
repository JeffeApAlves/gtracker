/*
 * gps.c
 *
 *  Created on: 22 de set de 2017
 *      Author: jefferson
 */
#include "consumer.h"
#include "gps.h"

static const TickType_t	xTaskDelay	= (1000 / portTICK_PERIOD_MS);

// Handles Tasks
TaskHandle_t	xHandleGPSTask	= NULL;

// Handles da Queue o GPS onde sera publicado os dados lidos do protocol NMEA
QueueHandle_t	xQueueGPS;

static DataGPS	gps;

void gps_task(void){

	while(1){

		if(gps_getdata()){

			gps_sendResultQueue();
		}

        vTaskDelay(xTaskDelay);
	}
}
//----------------------------------------------------------------------------------------

bool gps_getdata(void){

	gps_random_data();

	return true;
}
//----------------------------------------------------------------------------------------

void gps_init(void){

	xQueueGPS	= xQueueCreate( 5, sizeof( DataGPS ));

	xTaskCreate(
				gps_task, /* pointer to the task */
				"gps_task", /* task name for kernel awareness debugging */
				1024, /* task stack size */
				(void*)NULL, /* optional task startup argument */
				5, /* initial priority */
				&xHandleGPSTask /* optional task handle to create */
	);
}
//----------------------------------------------------------------------------------------

void gps_random_data(void){

	gps.Lat		= -23.591387;
	gps.Lng		= -46.645126;
	gps.Speed	= 50 + (rand() % 10);

	strcpy(gps.Date,"210917");
	strcpy(gps.Time_UTC,"152600");
}
//------------------------------------------------------------------------------------

void gps_sendResultQueue(void){

    if(xQueueSendToBack( xQueueGPS ,(void*) &gps, ( TickType_t ) 1 ) ){

    	//xTaskNotify( xHandleConsumer , BIT_UPDATE_GPS , eSetBits );
    	xEventGroupSetBits(consumer_event, BIT_UPDATE_GPS);
    }
}
//------------------------------------------------------------------------
