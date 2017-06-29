/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "MMA1.h"
#include "AppQueues.h"
#include "Accelerometer.h"

DataAcce	acceInfo;
DataAcce*	pAcceInfo = &acceInfo;

void read_accel(void) {

	MMA1_GetRaw8XYZ(pAcceInfo->AcceXYZ);

//	pAcceInfo->AcceXYZ[AXIS_X]	= 20;
//	pAcceInfo->AcceXYZ[AXIS_Y]	= 21;
//	pAcceInfo->AcceXYZ[AXIS_Z]	= 22;

	pAcceInfo->IncXYZ[AXIS_X]	= 30;
	pAcceInfo->IncXYZ[AXIS_Y]	= 31;
	pAcceInfo->IncXYZ[AXIS_Z]	= 32;

    if(xQueueSendToBack( xQueueAcce , ( void * ) &pAcceInfo, ( TickType_t ) 1 ) ){

    	xTaskNotifyGive( xHandleMainTask );

//TODO Sucesso da transmissao do valor dos acelerometros
    }
}
//------------------------------------------------------------------------

void initAccel(void){

	clearAcce(pAcceInfo);
	MMA1_Init();
	MMA1_Enable();
}
//------------------------------------------------------------------------
