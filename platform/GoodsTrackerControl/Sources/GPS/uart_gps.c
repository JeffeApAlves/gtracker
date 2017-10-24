/*
 * gps_serial.c
 *
 *  Created on: Oct 23, 2017
 *      Author: Jefferson
 */

#include "RingBuffer.h"

#include "uart_gps.h"



static RingBuffer			bufferRx;



inline bool getGPSData(char* ch){

	return getData(&bufferRx,ch);
}
//------------------------------------------------------------------------

inline bool putGPSData(char data) {

	return putData(&bufferRx,data);
}
//------------------------------------------------------------------------

inline bool isAnyGPSData(){

	return getCount(&bufferRx)>0;
}
//------------------------------------------------------------------------

void uart_gps_init(void){

	clearBuffer(&bufferRx);
}
//------------------------------------------------------------------------


inline uint16_t uart_gps_rx_head(void){

	return bufferRx.index_producer;
}
//------------------------------------------------------------------------

inline uint16_t uart_gps_rx_tail(void){

	return bufferRx.index_consumer;
}
//------------------------------------------------------------------------

inline uint16_t uart_gps_rx_max(void){

	return bufferRx.max_count;
}
//------------------------------------------------------------------------

