/*
 * serial.h
 *
 *  Created on: 23 de set de 2017
 *      Author: jefferson
 */


#ifndef COMPONENTS_COMMUNICATION_INCLUDE_SERIAL_H_
#define COMPONENTS_COMMUNICATION_INCLUDE_SERIAL_H_

#include <stdbool.h>

#include "RingBuffer.h"

#define SERIAL_DRIVER_ESP32

#define PIN_RXD  (16)
#define PIN_TXD  (17)
#define PIN_RTS  (18)
#define PIN_CTS  (19)

//Numero da uart que esta sendo utilizada para comunicacao com o Arm M0
#define	UART_NUM UART_NUM_2
#define	BUF_SIZE (1024)

void uart_init(void);
bool uart_wr_bytes(const char* data, size_t len);
bool uart_wr_data(const char data);
bool uart_wr_string(const char* data);

//API
bool getRxData(char* ch);
bool putTxData(char data);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
bool isAnyRxData();
bool putTxString(const char* str);
void startTX(void);

extern RingBuffer	bufferTx;

#endif /* COMPONENTS_COMMUNICATION_INCLUDE_SERIAL_H_ */
