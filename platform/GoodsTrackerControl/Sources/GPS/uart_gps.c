/*
 * uart_gps.c
 *
 *  Created on: Oct 16, 2017
 *      Author: Jefferson
 */

#include "uart_gps.h"

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

static device_uart_t	device;

static uart_rtos_config_t uart_config = {

	.baudrate = 4800,
    .parity = kUART_ParityDisabled,
    .stopbits = kUART_OneStopBit,
    .buffer = device.buffer,
    .buffer_size = sizeof(device.buffer),
};

#else
	static RingBuffer			bufferRx;
#endif

bool uart_gps_init(void){

	bool ret =false;

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	uart_config.srcclk	= UART_CLK_FREQ;
    uart_config.base	= UART;

    if (UART_RTOS_Init(&device.handle, &device.t_handle, &uart_config) == 0){

    	ret = true;
    }

#else
	clearBuffer(&bufferRx);
#endif

    return ret;
}
//---------------------------------------------------------------------------------------------

void uart_gps_Deinit(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

    UART_RTOS_Deinit(&device.handle);
#else

#endif
}
//---------------------------------------------------------------------------------------------

inline bool getGPSData(char* ch){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	size_t n;

	return UART_RTOS_Receive(&device.handle,(uint8_t*)ch, 1, &n)==kStatus_Success;
#else
	return getData(&bufferRx,ch);
#endif
}
//------------------------------------------------------------------------

inline bool putGPSData(char data) {

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return UART_RTOS_Send(&device.handle, (uint8_t*)&data, 1)==kStatus_Success;
#else
	return putData(&bufferRx,data);
#endif
}
//------------------------------------------------------------------------

inline bool isAnyGPSData(){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	//TODO como verifica se existe algum byte disponivel ?
	return true;
#else
	return getCount(&bufferRx)>0;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_gps_rx_head(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return device.t_handle.rxRingBufferHead;
#else
	return bufferRx.index_producer;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_gps_rx_tail(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return device.t_handle.rxRingBufferTail;
#else
	return bufferRx.index_consumer;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_gps_rx_max(void){

	static uint16_t max = 0;

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	if(device.t_handle.rxDataSize>max){

		max = device.t_handle.rxDataSize;
	}

#else
	max = bufferRx.max_count;
#endif

	return max;
}
//------------------------------------------------------------------------
