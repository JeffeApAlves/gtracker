/*
 * serial.h
 *
 *  Created on: Sep 27, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_UART_HOST_H_
#define INCLUDES_UART_HOST_H_

#include <stdint.h>
#include <stdbool.h>

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

uint16_t uart_host_rx_max(void);
uint16_t uart_host_rx_tail(void);
uint16_t uart_host_rx_head(void);

uint16_t uart_host_tx_max(void);
uint16_t uart_host_tx_tail(void);
uint16_t uart_host_tx_head(void);

#endif /* INCLUDES_UART_HOST_H_ */
