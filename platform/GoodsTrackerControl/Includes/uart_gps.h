/*
 * uart_gps.h
 *
 *  Created on: Oct 16, 2017
 *      Author: Jefferson
 */

#ifndef GPS_UART_GPS_H_
#define GPS_UART_GPS_H_

#include <stdbool.h>
#include <stdint.h>

#include "MCUC1.h"


#if MCUC1_CONFIG_NXP_SDK_2_0_USED

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"

#define GPS_SIZE_RING_BUFFER	128
#define UART					UART2
#define UART_CLKSRC				BUS_CLK
#define UART_CLK_FREQ			CLOCK_GetFreq(BUS_CLK)

typedef struct{

	uint8_t buffer[GPS_SIZE_RING_BUFFER];
	uart_rtos_handle_t		handle;
	struct _uart_handle		t_handle;
} device_uart_t;


#else

#include "RingBuffer.h"
#endif

bool uart_gps_init(void);
void uart_gps_Deinit(void);

bool getGPSData(char* ch);
bool putGPSData(char data);
bool isAnyGPSData();

uint16_t uart_gps_rx_max(void);
uint16_t uart_gps_rx_tail(void);
uint16_t uart_gps_rx_head(void);

#endif /* GPS_UART_GPS_H_ */
