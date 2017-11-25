/*
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */


#include "uart_gps.h"
#include "Telemetria.h"
#include "gps.h"

/* Task GPS*/
static const char*		GPS_TASK_NAME =			"tk_gps";
#define					GPS_NUM_MSG				1
#define 				GPS_TASK_PRIORITY		(tskIDLE_PRIORITY+2)
#define					GPS_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 100)
QueueHandle_t			xQueueGPS;
static TaskHandle_t		xHandleGPSTask;

EventGroupHandle_t		gps_events;

GPS	gps;

/**
 * Task de gerenciamento do GPS
 */
static portTASK_FUNCTION(task_gps, pvParameters) {

	while(1) {

		EventBits_t uxBits	= xEventGroupWaitBits(gps_events, GPS_BIT_RX_CHAR,pdTRUE,pdFALSE, portMAX_DELAY);

		if(uxBits & GPS_BIT_RX_CHAR){

			receiveNMEA();
		}
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

	}
}
//------------------------------------------------------------------------

void gps_publish(void){

    if(xQueueSendToBack( xQueueGPS ,(void*) &gps, ( TickType_t ) 1 ) == pdPASS){

    	tlm_notify_gps();
    }
}
//------------------------------------------------------------------------

inline BaseType_t gps_notify_rx_char(BaseType_t *xHigherPriorityTaskWoken){

	return xEventGroupSetBitsFromISR(gps_events, GPS_BIT_RX_CHAR,xHigherPriorityTaskWoken);
}
//------------------------------------------------------------------------------------


void gps_init(void){

	uart_gps_init();

	clearGPS(&gps);

	xQueueGPS	= xQueueCreate( GPS_NUM_MSG, sizeof( GPS ));

	gps_events	= xEventGroupCreate();

	createTask();
}
//-----------------------------------------------------------------------
