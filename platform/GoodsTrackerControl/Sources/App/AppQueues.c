/*
 * AppQueues.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include <CommunicationFrame.h>
#include <Frame.h>
#include "Telemetria.h"
#include "AppQueues.h"

// Tasks Delay
const TickType_t xMainDelay				= (60000 / portTICK_PERIOD_MS);
const TickType_t xCommunicationDelay	= (100 / portTICK_PERIOD_MS);
const TickType_t xDataDelay				= (500 / portTICK_PERIOD_MS);
const TickType_t xIHMDelay				= (100 / portTICK_PERIOD_MS);
const TickType_t xGPSDelay				= (1000 / portTICK_PERIOD_MS);
const TickType_t xAccelDelay			= (50 / portTICK_PERIOD_MS);

// Handles Tasks
TaskHandle_t 	xHandleMainTask		= NULL, xHandleCommunicationTask	= NULL,
				xHandleDataTask		= NULL, xHandleIHMTask				= NULL,
				xHandleGPSTask		= NULL,	xHandleAccelTask			= NULL,
				xHandleCallBackTask	= NULL, xHandleRunTxTask			= NULL;

// Handles das Queues
QueueHandle_t	xQueueCom, xQueueLCD, xQueueAnswer, xQueueGPS, xQueueAcc , xQueueTank;

/**
 * Cria todas as filas de menssagens utilizada na aplicação
 *
 */
void initQueues(void){

	xQueueCom		= xQueueCreate( 1, sizeof( CommunicationFrame ));
	xQueueAnswer	= xQueueCreate( 1, sizeof( CommunicationFrame ));
	xQueueGPS		= xQueueCreate( 1, sizeof( DataGPS ));
	xQueueAcc		= xQueueCreate( 1, sizeof( Accelerometer ));
	xQueueTank		= xQueueCreate( 1, sizeof( Tank ));
}
//-----------------------------------------------------------------------------
