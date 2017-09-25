/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "application.h"
#include "Tank.h"

static uint16_t	ADValues[AD1_CHANNEL_COUNT];
static Tank		adInfo;

volatile	bool AD_finished;

// Handles das Queue
QueueHandle_t	xQueueTank;

// Handles das Task
TaskHandle_t	xHandleDataTask;

static const TickType_t xTaskDelay	= (500 / portTICK_PERIOD_MS);

void tank_task(void){

	AD_finished = false;

	if(AD1_Measure(true)==ERR_OK){

		while (!AD_finished) {}

		if(AD1_GetValue16(&ADValues[0])==ERR_OK){

			adInfo.Level = ADValues[0];

		    if(xQueueSendToBack( xQueueTank ,  &adInfo, ( TickType_t ) 1 ) ){

		    	xTaskNotify( xHandleCallBackTask, BIT_UPDATE_AD , eSetBits );
		    }
		}
	}

	vTaskDelay(xTaskDelay);
}
//------------------------------------------------------------------------

void tank_init(void){

	AD_finished		= false;
	xQueueTank		= xQueueCreate( 1, sizeof( Tank ));
}
//-----------------------------------------------------------------------------