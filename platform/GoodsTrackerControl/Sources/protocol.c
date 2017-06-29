#include <stdlib.h>
#include <stdbool.h>

#include "XF1.h"
#include "Array.h"
#include "RingBuffer.h"
#include "protocol.h"

const char* OPERATION_AN = "AN";
const char* OPERATION_RD = "RD";
const char* OPERATION_WR = "WR";

StatusRx	statusRx = CMD_INIT;
RingBuffer	bufferRx,bufferTx;
DataCom		dataFrame;
ArrayFrame	frameCom;

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

/**
 *
 * Processamento da comunicacao
 *
 */
void processProtocol(void) {

	switch(statusRx){

		default:
		case CMD_INIT:			initRxCMD();			break;
		case CMD_INIT_OK:		rxStartCMD();			break;
		case CMD_RX_START:		receiveFrame();			break;
		case CMD_RX_FRAME:		receiveFrame();			break;
		case CMD_RX_END:		rxCR();					break;
		case CMD_RX_CR:			rxLF();					break;
		case CMD_RX_LF:			verifyFrame();			break;
		case CMD_FRAME_OK:		acceptRxFrame();		break;
		case CMD_FRAME_NOK:		errorRxFrame();			break;
		case CMD_EXEC:			sendResult();			break;
		case CMD_EXEC_ERROR:	errorExec();			break;
	}
}
//------------------------------------------------------------------------

static void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_START){

			clearData(&dataFrame);
			clearArrayFrame(&frameCom);
			setStatusRx(CMD_RX_START);
		}
	}
}
//------------------------------------------------------------------------

static void receiveFrame (void) {

	char ch;

	if(getRxData(&ch)) {

		if(ch==CHAR_START || frameCom.Count>=SIZE_FRAME) {

			setStatusRx(CMD_FRAME_NOK);
		}
		else
		  if(ch==CHAR_END) {

			 if(frameCom.Count>=SIZE_MIN_FRAME) {

				setStatusRx(CMD_RX_END);
			 }
			 else {

			   setStatusRx(CMD_FRAME_NOK);
			 }
		}
		else {

			putDataArray(&frameCom,ch);
			setStatusRx(CMD_RX_FRAME);
		}
	}
}
//------------------------------------------------------------------------

static void rxLF(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_LF){

			setStatusRx(CMD_RX_LF);

		}else {

			setStatusRx(CMD_FRAME_NOK);
		}
	}
}
//------------------------------------------------------------------------

static void rxCR(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_CR){

			setStatusRx(CMD_RX_CR);

		}else{

			setStatusRx(CMD_FRAME_NOK);
		}
	}
}
//------------------------------------------------------------------------

static void verifyFrame(void) {

	if(decoderFrame()){

		setStatusRx(CMD_FRAME_OK);

	}else{

		setStatusRx(CMD_FRAME_NOK);
	}
}
//------------------------------------------------------------------------

static bool decoderFrame(void) {

	bool ret = false;

	List	list;

	str_split(&list, frameCom.Data, CHAR_SEPARATOR);

	if(list.itens!=NULL) {

		// Minimo 8 itens
		if(list.count >= 8){

			//-2 para desconsiderar o checksum que esta no frame recebido
			unsigned int checksum_rx;
			unsigned int checksum_calc = calcChecksum(frameCom.Data, frameCom.Count - LEN_CHECKSUM);
			checksum_rx = ~checksum_calc;

			AsHex(&checksum_rx,&list,list.count-1);

			if(checksum_rx==checksum_calc) {

				AsInteger(&dataFrame.address,		&list,0);
				AsInteger(&dataFrame.dest,			&list,1);
				AsInteger(&dataFrame.countFrame,	&list,2);
				AsString(&dataFrame.operacao,		&list,3);
				AsString(&dataFrame.resource,		&list,4);
				AsInteger(&dataFrame.PayLoad.Count,	&list,5);
				AsString(&dataFrame.PayLoad.Data,	&list,6);

				ret = true;
			}
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

static pCallBack getCallBack(void) {

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

static void errorExec(void) {

	/*
	 * TBD
	 */
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

/**
 *
 * Envia o  resultado
 *
 */
static void sendResult(void){

	sendFrame(&dataFrame);

	// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX
	startTX();

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

/**
 *
 * Envia o frame
 *
 */
static void sendFrame(DataCom* frame){

	// Envia caracter de inicio
	putTxData(CHAR_START);

	// Emvia o frame
	sendString(frameCom.Data);

	// Envia o caracter de fim
	putTxData(CHAR_END);

	// Envia caracteres de controle
	putTxData(CHAR_CR);
	putTxData(CHAR_LF);
}
//------------------------------------------------------------------------

static void sendString(const char* str){

	char* p = (char *)str;

	if(p!=NULL){

		while(*p!=CHAR_STR_END){

			putTxData(*p++);
		}
	}
}
//------------------------------------------------------------------------

static inline void setStatusRx(StatusRx sts) {

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

/*
 *
 * Pega um char do buffer circular da transmissao
 *
 */
inline bool getTxData(char* ch){

	return getData(&bufferTx,ch);
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

/**
 * Recepcao do frame OK
 *
 */
static void acceptRxFrame(void) {

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
static void errorRxFrame(void){

	/**/

	setStatusRx(CMD_INIT_OK);
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
 *
 * Constroi o frame para envio
 *
 */
static void buildFrame(DataCom *frame) {

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
static void AppendHeader(DataCom *frame) {

	XF1_xsprintf(frameCom.Data,"%05d%c%05d%c%05d%c%s%c%s%c%03d%c",
				frame->address, 		CHAR_SEPARATOR,
				frame->dest, 			CHAR_SEPARATOR,
				frame->countFrame,		CHAR_SEPARATOR,
				frame->operacao, 		CHAR_SEPARATOR,
				frame->resource, 		CHAR_SEPARATOR,
				frame->PayLoad.Count,	CHAR_SEPARATOR);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o Payload no Frame
 *
 */
inline static void AppendPayLoad(DataCom* frame) {

	strncat(frameCom.Data, frame->PayLoad.Data,frame->PayLoad.Count);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o CheckSum no Frame
 *
 */
static void AppendCheckSum(DataCom* frame) {

	char	separator[] = {CHAR_SEPARATOR,CHAR_STR_END};
	char	checksum[LEN_CHECKSUM+1];

	strcat(frameCom.Data,separator);
	frameCom.Count = strlen(frameCom.Data);
	XF1_xsprintf(checksum, "%02X", calcChecksum (frameCom.Data,frameCom.Count));
	strcat(frameCom.Data, checksum);
}
//------------------------------------------------------------------------

/*
 *
 * Set PayLoad
 *
 */
static void setPayLoad(DataCom* frame, char* str) {

	size_t size = strlen(str);

	if(frame->PayLoad.Count <= SIZE_PAYLOAD){

		strcpy(frame->PayLoad.Data, str);

	}else{

		//TODO Erro: mensagem maior que o array do payload
	}

	frame->PayLoad.Count	= strlen(frame->PayLoad.Data);
}
//------------------------------------------------------------------------

/**
 *
 * Funcao disponibilizada para aplicacao preencher o payload
 *
 */
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
static void startTX(void){

	vPortEnterCritical();

	if(hasTxData() && AS1_GetCharsInTxBuf()==0){

		AS1_OnTxChar();
	}

	vPortExitCritical();
}
//------------------------------------------------------------------------

/**
 *
 * Inicializa a comunicacao
 *
 */
void initRxCMD(void) {

	clearData(&dataFrame);
	clearArrayFrame(&frameCom);
	clearBuffer(&bufferRx);
	clearBuffer(&bufferTx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------
