/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "DataTLM.h"
#include "AppQueues.h"
#include "MMA8451.h"
#include "Accelerometer.h"

static DataTLM	acceInfo;
DataTLM*	pAcceInfo = &acceInfo;

void runAccelerometer(void) {

	MMA845x_getValues(pAcceInfo->Axis);

    if(xQueueSendToBack( xQueueDataTLM , ( void * ) &pAcceInfo, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleCallBackTask , BIT_UPDATE_ACCE , eSetBits );
    }
}
//------------------------------------------------------------------------

void initAccelerometer(void){

	clearDataTLM(pAcceInfo);
	MMA845x_init();
}
//------------------------------------------------------------------------

void deInitAccelerometer(void){

	MMA845x_deInit();
}
//------------------------------------------------------------------------
