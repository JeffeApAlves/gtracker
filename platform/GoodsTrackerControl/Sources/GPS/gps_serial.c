/*
 * gps_serial.c
 *
 *  Created on: Oct 23, 2017
 *      Author: Jefferson
 */


#include "RingBuffer.h"

#include "gps_serial.h"


RingBuffer			bufferRxNMEA;



inline bool getGPSData(char* ch){

	return getData(&bufferRxNMEA,ch);
}
//------------------------------------------------------------------------

inline bool putGPSData(char data) {

	return putData(&bufferRxNMEA,data);
}
//------------------------------------------------------------------------

inline bool isAnyGPSData(){

	return getCount(&bufferRxNMEA)>0;
}
//------------------------------------------------------------------------

void gps_serial_init(void){

	clearBuffer(&bufferRxNMEA);
}
//------------------------------------------------------------------------
