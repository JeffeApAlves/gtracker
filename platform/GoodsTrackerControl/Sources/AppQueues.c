/*
 * AppQueues.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "AppQueues.h"
#include "Level.h"
#include "Accelerometer.h"
#include "NMEAFrame.h"
#include "DataFrame.h"

// Handles Tasks
TaskHandle_t xHandleMainTask = NULL, xHandleCommunicationTask = NULL,
		xHandleDataTask = NULL, xHandleIHMTask = NULL, xHandleGPSTask = NULL,
		xHandleAccelTask = NULL;

QueueHandle_t	xQueueGPS,xQueueCom,xQueueAcce,xQueueLCD,xQueuePayload,xQueueAD;

//static TaskHandle_t xTaskToNotify = NULL;

void initQueues(void){

	xQueueGPS		= xQueueCreate( 2, sizeof( DataNMEA* ) );
	xQueueCom		= xQueueCreate( 2, sizeof( DataCom* ) );
	xQueueAcce		= xQueueCreate( 2, sizeof( DataAcce* ) );
	xQueuePayload	= xQueueCreate( 2, sizeof( char* ) );
	xQueueAD		= xQueueCreate( 2, sizeof( DataAD* ) );
}
//-----------------------------------------------------------------------------
