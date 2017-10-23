/*
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */

#include "application.h"
#include "NMEA.h"
#include "gps.h"

/* Task GPS*/
static const char*		GPS_TASK_NAME =			"tk_gps";
#define					GPS_NUM_MSG				1
#define 				GPS_TASK_PRIORITY		(tskIDLE_PRIORITY+2)
#define					GPS_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 100)
static const TickType_t GPS_TASK_DELAY	= 		(200 / portTICK_PERIOD_MS);
QueueHandle_t			xQueueGPS;
TaskHandle_t			xHandleGPSTask;

GPS	gps;

/**
 * Task de gerenciamento do GPS
 */
static portTASK_FUNCTION(task_gps, pvParameters) {

	while(1) {

		receiveNMEA();

		vTaskDelay(GPS_TASK_DELAY);
	}

	vTaskDelete(xHandleGPSTask);
}
//--------------------------------------------------------------------------------------

static void createTask(void){

	if (xTaskCreate(
		task_gps,
		GPS_TASK_NAME,
		GPS_TASK_STACK_SIZE,
		(void*)NULL,
		GPS_TASK_PRIORITY,
		&xHandleGPSTask
	) != pdPASS) {

		while(1) {};
	}
}
//------------------------------------------------------------------------

void gps_publish(void){

    if(xQueueSendToBack( xQueueGPS ,(void*) &gps, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleAppTask , BIT_UPDATE_GPS , eSetBits );
    }
}
//------------------------------------------------------------------------

void gps_init(void){

	clearGPS(&gps);

	xQueueGPS		= xQueueCreate( GPS_NUM_MSG, sizeof( GPS ));

	createTask();
}
//-----------------------------------------------------------------------
