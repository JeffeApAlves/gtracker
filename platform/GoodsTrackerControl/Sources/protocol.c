#include <stdlib.h>
#include <stdbool.h>

#include "XF1.h"
#include "RingBuffer.h"
#include "protocol.h"

const char* OPERATION_AN = "AN";
const char* OPERATION_RD = "RD";
const char* OPERATION_WR = "WR";

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
		case CMD_RX_NL:			verifyCheckSum();		break;
		case CMD_DECODER:		acceptRxFrame();		break;
		case CMD_EXEC:			sendResult();			break;
		case CMD_ERROR:			errorRxFrame();			break;
		case CMD_EXEC_ERROR:	errorExec();			break;
	}
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

	char ch;

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

		  dataFrame.frame[(dataFrame.sizeFrame++)%SIZE_MAX_FRAME] = ch;
		  setStatusRx(CMD_RX_PAYLOAD);
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

void verifyCheckSum(void) {

	if(decoderFrame()){

		setStatusRx(CMD_DECODER);

	}else{

		setStatusRx(CMD_ERROR);
	}
}
//------------------------------------------------------------------------

bool decoderFrame(void) {

	bool ret = false;

	List	list;

	str_split(&list, dataFrame.frame, CHAR_SEPARATOR);

	// Minimo 8 itens
	if(list.itens!=NULL && list.count >= 8) {

		//-2 para desconsiderar o checksum que esta no frame recebido
		unsigned int checksum_rx;
		unsigned int checksum_calc = calcChecksum(dataFrame.frame, dataFrame.sizeFrame - LEN_CHECKSUM);
		checksum_rx = ~checksum_calc;

		AsHex(&checksum_rx,&list,list.count-1);

		if(checksum_rx==checksum_calc) {

			char i;

			AsInteger(&dataFrame.address,		&list,0);
			AsInteger(&dataFrame.dest,			&list,1);
			AsInteger(&dataFrame.count,			&list,2);
			AsString(&dataFrame.operacao,		&list,3);
			AsString(&dataFrame.resource,		&list,4);
			AsInteger(&dataFrame.sizePayLoad,	&list,5);
			AsString(&dataFrame.payload,		&list,6);

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

	sendFrame(&dataFrame);

	// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX
	startTX();

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendFrame(DataFrame* frame){

	putTxData(CHAR_CMD_START);

	sendString(frame->frame);

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

/**
 * Recepcao do frame OK
 *
 */
void acceptRxFrame(void) {

	pCallBack cb = getCallBack();

	if(cb!=NULL && cb(&dataFrame) == CMD_RESULT_EXEC_SUCCESS) {

		setStatusRx(CMD_EXEC);
	}
	else {

		setStatusRx(CMD_EXEC_ERROR);
	}
}
//------------------------------------------------------------------------

/**
 * Erro na recepcao do frame
 *
 */
void errorRxFrame(void){

	/**/

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

/**
 * Verifica se existe dados no buffer circular de transmiassa
 */
bool hasTxData(void){

	return hasData(&bufferTx);
}
//------------------------------------------------------------------------

void buildFrame(DataFrame *frame) {

	AppendHeader(frame);
	AppendPayLoad(frame);
	AppendCheckSum(frame);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o cabecalho no frame
 *
 */
void AppendHeader(DataFrame *frame) {

	XF1_xsprintf(frame->frame,"%05d%c%05d%c%05d%c%s%c%s%c%03d%c",
				frame->address, CHAR_SEPARATOR,
					frame->dest, CHAR_SEPARATOR,
						frame->count, CHAR_SEPARATOR,
							frame->operacao, CHAR_SEPARATOR,
								frame->resource, CHAR_SEPARATOR,
									frame->sizePayLoad, CHAR_SEPARATOR);

	frame->sizeHeader	= strlen(frame->frame);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o Payload no Frame
 *
 */
void AppendPayLoad(DataFrame *frame) {

	strncat(frame->frame, frame->payload,frame->sizePayLoad);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o CheckSum no Frame
 *
 */
void AppendCheckSum(DataFrame *frame) {

	char	separator[] = {CHAR_SEPARATOR,CHAR_STR_END};
	char	checksum[LEN_CHECKSUM+1];

	strcat(frame->frame,separator);
	frame->sizeFrame = strlen(frame->frame);
	XF1_xsprintf(checksum, "%02X", calcChecksum (frame->frame,frame->sizeFrame));
	strcat(frame->frame, checksum);
}
//------------------------------------------------------------------------

/*
 *
 * Set PayLoad
 *
 *
 */
void setPayLoad(DataFrame* frame, char* str) {

	size_t size = strlen(str);

	if(frame->sizePayLoad <= SIZE_MAX_PAYLOAD){

		strcpy(frame->payload, str);

	}else{

		//TODO Erro: mensagem maior que o array do payload
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

unsigned int calcChecksum(const char *buff, size_t sz) {

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

	memset(frame,0,sizeof(DataFrame));
}
//------------------------------------------------------------------------
