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

#include "MCUC1.h"

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"

#define UART_HOST_SIZE_RING_BUFFER	128
#define UART_HOST					UART1
#define UART_HOST_CLKSRC			BUS_CLK
#define UART_HOST_CLK_FREQ			CLOCK_GetFreq(UART_HOST_CLKSRC)
#define UART_HOST_BAUD 				57600

typedef struct{

	uint8_t buffer[UART_HOST_SIZE_RING_BUFFER];
	uart_rtos_handle_t		handle;
	struct _uart_handle		t_handle;
} host_uart_t;


#else
#include "RingBuffer.h"
#include "Events.h"
#include "AS1.h"
#endif


bool getRxData(char* ch);
bool putTxData(char data);
bool putRxData(char ch);
bool getTxData(char* ch);
bool isAnyRxData();
bool isAnyTxData();
bool getRxData(char* ch);
bool putTxString(char* str);
void startTX(void);
bool uart_host_init(void);
void uart_host_Deinit(void);

uint16_t uart_host_rx_max(void);
uint16_t uart_host_rx_tail(void);
uint16_t uart_host_rx_head(void);

uint16_t uart_host_tx_max(void);
uint16_t uart_host_tx_tail(void);
uint16_t uart_host_tx_head(void);

#endif /* INCLUDES_UART_HOST_H_ */
