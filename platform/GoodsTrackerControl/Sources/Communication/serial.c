/*
 * serial.c
 *
 *  Created on: Sep 27, 2017
 *      Author: Jefferson
 */
#include <stdio.h>

#include "Events.h"
#include "AS1.h"

#include "serial.h"

RingBuffer			bufferRx,bufferTx;


inline bool putTxData(char data) {

#ifdef SERIAL_DRIVER_ESP32
	return uart_wr_data(data);
#else
	return putData(&bufferTx,data);
#endif
}
//------------------------------------------------------------------------

inline bool putRxData(char data) {

	return putData(&bufferRx,data);
}
//------------------------------------------------------------------------

/*
 *
 * Pega um char do buffer circular da transmissao
 *
 */
inline bool getTxData(char* ch){

	return getData(&bufferTx,ch);
}
//------------------------------------------------------------------------


/**
 * Verifica se existe dados no buffer circular de transmiassa
 */
inline bool hasTxData(void){

	return hasData(&bufferTx);
}
//------------------------------------------------------------------------


/**
 * Verifica se o buffer de TX esta vazio. Se sim, chama a call back
 * da transmissao de caracter para iniciar a transmissao do buffer
 *
 */
void startTX(void){

	vPortEnterCritical();

	if(hasTxData() && AS1_GetCharsInTxBuf()==0){

		AS1_OnTxChar();
	}

	vPortExitCritical();
}
//------------------------------------------------------------------------

/*
 *
 * Verifica se exsite algum dado no buffer circular de recebimento
 *
 */
inline bool isAnyRxData(){

	return getCount(&bufferRx)>0;
}
//------------------------------------------------------------------------


/*
 *
 * Pega um char do buffer circular da recepcao
 *
 */
inline bool getRxData(char* ch){

	return getData(&bufferRx,ch);
}
//------------------------------------------------------------------------

inline void putTxString(char* str){

	putString(&bufferTx,str);
}
//------------------------------------------------------------------------

void uart_init(void){

	clearBuffer(&bufferRx);
	clearBuffer(&bufferTx);
}
//------------------------------------------------------------------------
