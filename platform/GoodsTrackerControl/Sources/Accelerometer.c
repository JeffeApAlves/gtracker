/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "DataTLM.h"
#include "MMA1.h"
#include "AppQueues.h"
#include "Accelerometer.h"

DataTLM		acceInfo;
DataTLM*	pAcceInfo = &acceInfo;

void runAccelerometer(void) {

//	MMA1_GetRaw8XYZ(&pAcceInfo->Acc[0]);

	pAcceInfo->Acc[AXIS_X] = MMA1_GetXmg();
	pAcceInfo->Acc[AXIS_Y] = MMA1_GetYmg();
	pAcceInfo->Acc[AXIS_Z] = MMA1_GetZmg();
//	pAcceInfo->AcceXYZ[AXIS_X]	= 20;
//	pAcceInfo->AcceXYZ[AXIS_Y]	= 21;
//	pAcceInfo->AcceXYZ[AXIS_Z]	= 22;

	pAcceInfo->Inc[AXIS_X]	= 30;
	pAcceInfo->Inc[AXIS_Y]	= 31;
	pAcceInfo->Inc[AXIS_Z]	= 32;

    if(xQueueSendToBack( xQueueDataTLM , ( void * ) &pAcceInfo, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleMainTask , BIT_UPDATE_ACCE , eSetBits );
    }
}
//------------------------------------------------------------------------

void initAccel(void){

	if(MMA1_Init()==ERR_OK){

		MMA1_Enable();
	}

	clearDataTLM(pAcceInfo);
}
//------------------------------------------------------------------------