/*
 * serial.c
 *
 *  Created on: Sep 27, 2017
 *      Author: Jefferson
 */
#include <stdio.h>

#include "uart_host.h"

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

static host_uart_t	device;

static uart_rtos_config_t uart_config = {

	.baudrate = UART_HOST_BAUD,
    .parity = kUART_ParityDisabled,
    .stopbits = kUART_OneStopBit,
    .buffer = device.buffer,
    .buffer_size = sizeof(device.buffer),
};

#else
RingBuffer			bufferRx,bufferTx;
#endif


/**
 * Verifica se o buffer de TX esta vazio. Se sim, chama a call back
 * da transmissao de caracter para iniciar a transmissao do buffer
 *
 */
void startTX(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED==0

	vPortEnterCritical();

	if(isAnyTxData() && AS1_GetCharsInTxBuf()==0){

		AS1_OnTxChar();
	}

	vPortExitCritical();
#endif

}
//------------------------------------------------------------------------

inline bool putTxString(char* str){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return UART_RTOS_Send(&device.handle, (uint8_t*)&str,strlen(str))==kStatus_Success;
#else
	putString(&bufferTx,str);
	//TODO implementar retorno do buffer
	return true;
#endif
}
//------------------------------------------------------------------------

bool uart_host_init(void){

	bool ret =false;

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	uart_config.srcclk	= UART_HOST_CLK_FREQ;
    uart_config.base	= UART_HOST;

    if (UART_RTOS_Init(&device.handle, &device.t_handle, &uart_config) == 0){

    	ret = true;
    }

#else
	clearBuffer(&bufferRx);
	clearBuffer(&bufferTx);

#endif

    return ret;
}
//---------------------------------------------------------------------------------------------

void uart_host_Deinit(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

    UART_RTOS_Deinit(&device.handle);
#else

#endif
}
//---------------------------------------------------------------------------------------------

inline bool getTxData(char* ch){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	//TODO verifica a necessidade
	return 0;
#else
	return getData(&bufferTx,ch);
#endif
}
//------------------------------------------------------------------------

inline bool putTxData(char data) {

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return UART_RTOS_Send(&device.handle, (uint8_t*)&data, 1)==kStatus_Success;
#else
	return putData(&bufferTx,data);
#endif
}
//------------------------------------------------------------------------

inline bool isAnyTxData(){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	//TODO como verifica se existe algum byte disponivel ?
	return true;
#else
	return getCount(&bufferTx)>0;
#endif
}
//------------------------------------------------------------------------

inline bool getRxData(char* ch){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	size_t n=0;
	return UART_RTOS_Receive(&device.handle,(uint8_t*)ch, 1, &n)==kStatus_Success;
#else
	return getData(&bufferRx,ch);
#endif
}
//------------------------------------------------------------------------

inline bool putRxData(char data) {

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	//Verificar a necessidade
	return true;
#else
	return putData(&bufferRx,data);
#endif
}
//------------------------------------------------------------------------

inline bool isAnyRxData(){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	//TODO como verifica se existe algum byte disponivel ?
	return true;
#else
	return getCount(&bufferRx)>0;
#endif
}
//------------------------------------------------------------------------


inline uint16_t uart_host_rx_head(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return device.t_handle.rxRingBufferHead;
#else
	return bufferRx.index_producer;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_host_rx_tail(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return device.t_handle.rxRingBufferTail;
#else
	return bufferRx.index_consumer;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_host_rx_max(void){

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

inline uint16_t uart_host_tx_head(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return 0;
#else
	return bufferTx.index_producer;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_host_tx_tail(void){


#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return 0;
#else
	return bufferTx.index_consumer;
#endif
}
//------------------------------------------------------------------------

inline uint16_t uart_host_tx_max(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return 0;
#else
	return bufferTx.max_count;
#endif
}
//------------------------------------------------------------------------
