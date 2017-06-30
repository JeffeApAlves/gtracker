/*
 * AppQueues.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "Array.h"
#include "Data.h"
#include "DataFrame.h"
#include "AppQueues.h"

// Tasks Delay
const TickType_t xMainDelay = 50 / portTICK_PERIOD_MS;
const TickType_t xCommunicationDelay = 5 / portTICK_PERIOD_MS;
const TickType_t xDataDelay = 10 / portTICK_PERIOD_MS;
const TickType_t xIHMDelay = 25 / portTICK_PERIOD_MS;
const TickType_t xGPSDelay = 5 / portTICK_PERIOD_MS;
const TickType_t xAccelDelay = 10 / portTICK_PERIOD_MS;


// Handles Tasks
TaskHandle_t xHandleMainTask = NULL, xHandleCommunicationTask = NULL,
		xHandleDataTask = NULL, xHandleIHMTask = NULL, xHandleGPSTask = NULL,
		xHandleAccelTask = NULL;

// Queues
QueueHandle_t	xQueueCom,xQueueLCD,xQueuePayload,xQueueData;

uint32_t ulPreviousValue;

void initQueues(void){

	xQueueCom		= xQueueCreate( 2, sizeof( DataCom* ));
	xQueuePayload	= xQueueCreate( 2, sizeof( ArrayPayLoad* ));
	xQueueData		= xQueueCreate( 5, sizeof( Info* ));
}
//-----------------------------------------------------------------------------
