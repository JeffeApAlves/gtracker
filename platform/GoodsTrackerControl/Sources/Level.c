/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "AppQueues.h"
#include "Level.h"

uint16_t	ADValues[AD1_CHANNEL_COUNT];

DataAD		adInfo;
DataAD*		pInfo = &adInfo;

volatile	bool AD_finished = FALSE;

void read_Channels_AD(void){

	if(AD1_Measure(TRUE)==ERR_OK){

		while (!AD_finished) {}

		if(AD1_GetValue16(ADValues)==ERR_OK){

			pInfo->Level = ADValues[2];

		    if(xQueueSendToBack( xQueueAD , ( void * ) &pInfo, ( TickType_t ) 1 ) ){

		    	xTaskNotifyGive( xHandleMainTask );

		//TODO Sucesso da transmissao do valor dos acelerometros
		    }
		}
	}
}
//------------------------------------------------------------------------



