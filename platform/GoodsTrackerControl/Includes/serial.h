/*
 * serial.h
 *
 *  Created on: Sep 27, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_SERIAL_H_
#define INCLUDES_SERIAL_H_


#include "RingBuffer.h"

bool getRxData(char* ch);
bool putTxData(char data);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
bool isAnyRxData();
bool getRxData(char* ch);
void putTxString(char* str);
void startTX(void);
void uart_init(void);

//Para debug apenas
extern RingBuffer	bufferRx,bufferTx;

#endif /* INCLUDES_SERIAL_H_ */
