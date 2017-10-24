/*
 * gps_serial.h
 *
 *  Created on: Oct 23, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_UART_GPS_H_
#define INCLUDES_UART_GPS_H_

#include <stdbool.h>
#include <stdint.h>

bool getGPSData(char* ch);
bool putGPSData(char data);
bool isAnyGPSData();
void uart_gps_init(void);

uint16_t uart_gps_rx_max(void);
uint16_t uart_gps_rx_tail(void);
uint16_t uart_gps_rx_head(void);

#endif /* INCLUDES_UART_GPS_H_ */
