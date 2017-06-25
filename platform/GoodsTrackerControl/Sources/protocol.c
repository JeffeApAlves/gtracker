#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "RingBuffer.h"
#include "utils.h"
#include "protocol.h"

const char* OPERATION_AN="AN";
const char* OPERATION_RD="RD";
const char* OPERATION_WR="WR";

unsigned int timeTx = TIME_TX;
unsigned int timeRx = TIME_RX;

StatusRx	statusRx = CMD_INIT;
RingBuffer	bufferRx,bufferTx;
DataFrame	dataFrame;

Resource	ListCmd[]	= {	{.resourceID = CMD_LED,			.resource = "LED\0",	.cb = NULL},
							{.resourceID = CMD_ANALOG,		.resource = "ANL\0",	.cb = NULL},
							{.resourceID = CMD_PWM,			.resource = "PWM\0",	.cb = NULL},
							{.resourceID = CMD_ACC,			.resource = "ACC\0",	.cb = NULL},
							{.resourceID = CMD_TOUCH,		.resource = "TOU\0",	.cb = NULL},
							{.resourceID = CMD_TELEMETRIA,	.resource = "TLM\0",	.cb = NULL},
							{.resourceID = CMD_LOCK,		.resource = "LCK\0",	.cb = NULL},
							{.resourceID = CMD_LCD,			.resource = "LCD\0",	.cb = NULL},
				};

const unsigned char SIZE_LIST_CMD = sizeof(ListCmd)/sizeof(Resource);

void processProtocol(void) {

	switch(statusRx){

		default:
		case CMD_INIT:			initRxCMD();			break;
		case CMD_INIT_OK:		rxStartCMD();			break;
		case CMD_RX_START:		receiveFrame();			break;
		case CMD_RX_PAYLOAD:	receiveFrame();			break;
		case CMD_RX_END:		rxCR();					break;
		case CMD_RX_CR:			rxNL();					break;
		case CMD_RX_NL:			decoderFrame();			break;
		case CMD_DECODER:		execCallBack();			break;

		case CMD_ERROR:			errorRx();				break;
		case CMD_EXEC:			sendResult();			break;
		case CMD_EXEC_ERROR:	errorExec();			break;
	}

	// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX
	startTX();
}
//------------------------------------------------------------------------

void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_CMD_START){

			clearData(&dataFrame);

			setStatusRx(CMD_RX_START);
		}
	}
}
//------------------------------------------------------------------------

void receiveFrame (void) {

	unsigned char ch;

	if(getRxData(&ch)) {

		if(ch==CHAR_CMD_START || dataFrame.sizeFrame>=SIZE_MAX_FRAME) {
			setStatusRx(CMD_ERROR);
		}
		else
		  if(ch==CHAR_CMD_END) {

			 if(dataFrame.sizeFrame>=SIZE_MIN_FRAME) {

				setStatusRx(CMD_RX_END);
			 }
			 else {
			   setStatusRx(CMD_ERROR);
			 }
		}
		else {

		  setStatusRx(CMD_RX_PAYLOAD);
		  dataFrame.frame[(dataFrame.sizeFrame++)%SIZE_MAX_FRAME] = ch;
		}
	}
}
//------------------------------------------------------------------------

void rxNL(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_LF){

			setStatusRx(CMD_RX_NL);

		}else {

			setStatusRx(CMD_ERROR);
		}
	}
}
//------------------------------------------------------------------------

void rxCR(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_CR){

			setStatusRx(CMD_RX_CR);

		}else{

			setStatusRx(CMD_ERROR);
		}
	}
}
//------------------------------------------------------------------------

void decoderFrame(void) {

	if(decoderFrame2()){

		setStatusRx(CMD_DECODER);

	}else{

		setStatusRx(CMD_ERROR);
	}
}
//------------------------------------------------------------------------


bool decoderFrame2(void) {

	bool ret = false;

	List	list;

	//-2 para desconsiderar o checksum que que esta no frame recebido
	unsigned char checksum_calc = calcChecksum(dataFrame.frame, dataFrame.sizeFrame - LEN_CHECKSUM);

	str_split(&list, dataFrame.frame, CHAR_SEPARATOR);

	if(list.itens!=NULL) {


		/*Usamos strtol pois o último objeto da lista armazena o valor do checksum em HEXA*/
		dataFrame.checksum_rx = strtol(list.itens[list.size-1], NULL, 16);

		if(dataFrame.checksum_rx==checksum_calc) {

			char i;

			for(i=0;i < list.size;i++) {

				if(list.itens[i]!=NULL) {

					switch(i) {

						case 0:	dataFrame.address			= atoi(list.itens[0]);					break;
						case 1:	dataFrame.dest				= atoi(list.itens[1]);					break;
						case 2:	strncpy(dataFrame.operacao, list.itens[2], strlen(list.itens[2]));	break;
						case 3:	strncpy(dataFrame.resource,	list.itens[3], strlen(list.itens[3]));	break;
						case 4:	dataFrame.sizePayLoad		= atoi(list.itens[4]);					break;
						case 5:	memcpy(dataFrame.payload, 	list.itens[5], dataFrame.sizePayLoad);	break;
					}
				}
			}

			ret = true;
		}

		removeList(&list);
	}

	return ret;
}
//------------------------------------------------------------------------

void setEventCMD(ResourceID id,pCallBack c) {

	int i;
	for(i=0;i<SIZE_LIST_CMD;i++){

		if(ListCmd[i].resourceID==id){

			ListCmd[i].cb = c;
			break;
		}
	}
}
//------------------------------------------------------------------------

pCallBack getCallBack(void) {

	pCallBack cb = NULL;

	int i;
	for(i = 0; i < SIZE_LIST_CMD; i++){

		if(strcmp(ListCmd[i].resource, dataFrame.resource) == 0){

			cb = ListCmd[i].cb;
			break;
		}
	}

	return cb;
}
//------------------------------------------------------------------------

void execCallBack(void) {

	pCallBack cb = getCallBack();

	if(cb!=NULL && cb(&dataFrame) == CMD_RESULT_EXEC_SUCCESS) {

		setStatusRx(CMD_EXEC);
	}
	else {

		setStatusRx(CMD_EXEC_ERROR);
	}
}
//------------------------------------------------------------------------

void initRxCMD(void) {

	clearData(&dataFrame);
	initBuffer(&bufferRx);
	initBuffer(&bufferTx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void errorExec(void) {

	/*
	 * TBD
	 */
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendResult(void){

	sendFrame(dataFrame.frame);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendFrame(const char* str){

	putTxData(CHAR_CMD_START);

	sendString(str);

	putTxData(CHAR_CMD_END);
	putTxData(CHAR_CR);
	putTxData(CHAR_LF);
}
//------------------------------------------------------------------------

void sendString(const char* str){

	char* p = (char *)str;

	if(p!=NULL){

		while(*p!=CHAR_STR_END){

			putTxData(*p++);
		}
	}
}
//------------------------------------------------------------------------

inline void setStatusRx(StatusRx sts) {

	statusRx = sts;
}
//------------------------------------------------------------------------

inline bool putTxData(char data) {

	return putData(&bufferTx,data);
}
//------------------------------------------------------------------------

inline bool putRxData(char data) {

	return putData(&bufferRx,data);
}
//------------------------------------------------------------------------

inline bool getTxData(char* ch){

	return getData(&bufferTx,ch);
}
//------------------------------------------------------------------------

inline bool getRxData(char* ch){

	return getData(&bufferRx,ch);
}
//------------------------------------------------------------------------

void errorRx(void){

	/**/

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

bool hasTxData(void){

	return hasData(&bufferTx);
}
//------------------------------------------------------------------------

/**
 * Verifica se o buffer de TX esta vazio. Se sim, chama a call back
 * da transmissao de caracter para iniciar a transmissao do buffer
 *
 */
void startTX(void){

	if(hasTxData() && AS1_GetCharsInTxBuf()==0){

		AS1_OnTxChar();
	}
}
//------------------------------------------------------------------------

void buildHeader(DataFrame *frame) {

	sprintf(frame->frame,"%05d%c%05d%c%s%c%s%c%03d%c",
				frame->address, CHAR_SEPARATOR,
					frame->dest, CHAR_SEPARATOR,
						frame->operacao, CHAR_SEPARATOR,
							frame->resource, CHAR_SEPARATOR,
								frame->sizePayLoad, CHAR_SEPARATOR);

	frame->sizeHeader	= strlen(frame->frame);
}
//------------------------------------------------------------------------

void buildFrame(DataFrame *frame) {

	char seperador[]= {CHAR_SEPARATOR,CHAR_STR_END};
	char checksum[LEN_CHECKSUM+2];

	// Header
	buildHeader(frame);

	// Payload
	strncat(frame->frame, frame->payload,frame->sizePayLoad);

	// CheckSum
	strcat(dataFrame.frame, seperador);
	frame->sizeFrame = strlen(frame->frame);
	sprintf(checksum, "%02X", calcChecksum (frame->frame,frame->sizeFrame));
	strcat(frame->frame, checksum);
}
//------------------------------------------------------------------------

void setPayLoad(DataFrame* frame, char* str) {

	size_t size = strlen(str);

	if(frame->sizePayLoad <= SIZE_MAX_PAYLOAD){

		strcpy(frame->payload, str);
	}

	frame->sizePayLoad	= strlen(frame->payload);
}
//------------------------------------------------------------------------

void doAnswer(char *msg) {

	if (msg) {

		setPayLoad(&dataFrame, msg);

		strncpy(dataFrame.operacao, OPERATION_AN, LEN_OPERATION);
		dataFrame.dest		= dataFrame.address;
		dataFrame.address	= ADDRESS;

		buildFrame(&dataFrame);
	}
	else {
		/*O QUE RESPONDE EM CASO DE MENSAGEM NULA ???*/
	}
}
//------------------------------------------------------------------------

unsigned char calcChecksum(const char *buff, size_t sz) {

	int i;
	unsigned char chk	= 0;

	if (buff) {

		for (i = 0; i < sz; i++){
			chk ^= buff[i];
		}
	}

	return chk;
}
//------------------------------------------------------------------------

void clearData(DataFrame* frame){

	frame->checksum_rx	= -1;
	frame->address		= 0;
	frame->sizeFrame	= 0;
	frame->sizePayLoad	= 0;
	frame->sizeHeader	= 0;

	int i;

	for(i=0;i<SIZE_MAX_PAYLOAD;i++){

		frame->payload[i]	= CHAR_STR_END;
	}

	for(i=0;i<SIZE_MAX_FRAME;i++){

		frame->frame[i]		= CHAR_STR_END;
	}
}
//------------------------------------------------------------------------
