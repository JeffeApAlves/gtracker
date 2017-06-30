#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "XF1.h"
#include "Array.h"
#include "RingBuffer.h"
#include "AppQueues.h"
#include "protocol.h"

const char* OPERATION_AN = "AN";
const char* OPERATION_RD = "RD";
const char* OPERATION_WR = "WR";

StatusRx	statusRx = CMD_INIT;
RingBuffer	bufferRx,bufferTx;
DataCom		dataCom;
DataCom*	pDataCom = &dataCom;
ArrayFrame	frameCom;

Resource	ListCmd[]	= {	{.id = CMD_NONE,		.name = "---\0"},
							{.id = CMD_LED,			.name = "LED\0"},
							{.id = CMD_ANALOG,		.name = "ANL\0"},
							{.id = CMD_PWM,			.name = "PWM\0"},
							{.id = CMD_ACC,			.name = "ACC\0"},
							{.id = CMD_TOUCH,		.name = "TOU\0"},
							{.id = CMD_TLM,			.name = "TLM\0"},
							{.id = CMD_LOCK,		.name = "LCK\0"},
							{.id = CMD_LCD,			.name = "LCD\0"},
				};

const unsigned char SIZE_LIST_CMD = sizeof(ListCmd)/sizeof(Resource);

/**
 *
 * Processamento da comunicacao
 *
 */
void runCommunication(void) {

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
//		case CMD_EXEC:			sendResult();			break;
//		case CMD_EXEC_ERROR:	errorExec();			break;
	}

	anyResult();
}
//------------------------------------------------------------------------

void anyResult(void){

	ArrayPayLoad* payload;

	if (xQueueReceive(xQueuePayload, &(payload), (TickType_t ) 1)) {

		doAnswer(payload);

		sendResult();
	}
}
//------------------------------------------------------------------------

static void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_START){

			clearData(&dataCom);
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

				AsInteger(&dataCom.address,			&list,0);
				AsInteger(&dataCom.dest,			&list,1);
				AsInteger(&dataCom.countFrame,		&list,2);
				AsString(&dataCom.operacao,			&list,3);
				AsString(&dataCom.resource.name,	&list,4);
				AsInteger(&dataCom.PayLoad.Count,	&list,5);
				AsString(&dataCom.PayLoad.Data,		&list,6);

				ret = true;
			}
		}

		removeList(&list);
	}

	return ret;
}
//------------------------------------------------------------------------
/*
void setEventCMD(ResourceID id,pCallBack c) {

	int i;
	for(i=0;i<SIZE_LIST_CMD;i++){

		if(ListCmd[i].id==id){

			ListCmd[i].cb = c;
			break;
		}
	}
}
//------------------------------------------------------------------------
*/
static Resource getResource(char* name) {

	Resource	r = ListCmd[0];

	unsigned char i;

	for(i = 1; i < SIZE_LIST_CMD; i++){

		if(strcmp(ListCmd[i].name, name) == 0){

			r = ListCmd[i];
			break;
		}
	}

	return r;
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

	sendFrame(&dataCom);

	// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX
	startTX();

//	setStatusRx(CMD_INIT_OK);
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

	pDataCom->resource = getResource(pDataCom->resource.name);

	if(xQueueSendToBack( xQueueCom , ( void * ) &pDataCom, ( TickType_t ) 1 ) ){

		xTaskNotify( xHandleCallBackTask , (uint32_t) 0  ,eIncrement);
	}

	setStatusRx(CMD_INIT_OK);
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
static void buildFrame() {

	copyHeaderToFrame();
	copyPayLoadToFrame();
	copyCheckSumToFrame();
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o cabecalho no frame
 *
 */
static void copyHeaderToFrame(void) {

	XF1_xsprintf(frameCom.Data,"%05d%c%05d%c%05d%c%s%c%s%c%03d%c",
				dataCom.address, 		CHAR_SEPARATOR,
				dataCom.dest, 		CHAR_SEPARATOR,
				dataCom.countFrame,	CHAR_SEPARATOR,
				dataCom.operacao, 	CHAR_SEPARATOR,
				dataCom.resource.name,CHAR_SEPARATOR,
				dataCom.PayLoad.Count,CHAR_SEPARATOR);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o Payload no Frame
 *
 */
inline static void copyPayLoadToFrame(void) {

	AppendFrame(&frameCom,dataCom.PayLoad.Data);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o CheckSum no Frame
 *
 */
static void copyCheckSumToFrame(void) {

	char	separator[] = {CHAR_SEPARATOR,CHAR_STR_END};

	AppendFrame(&frameCom,separator);
	XF1_xsprintf(frameCom.checksum, "%02X", calcChecksum (frameCom.Data,frameCom.Count));
	AppendFrame(&frameCom,frameCom.checksum);
}
//------------------------------------------------------------------------

/*
 *
 * Set PayLoad
 *
 */
static void setPayLoad(DataCom* frame, ArrayPayLoad* ans) {

	if(frame!=NULL && ans!=NULL){

		frame->PayLoad =  *ans;

		if(frame->PayLoad.Count > SIZE_PAYLOAD){

			//TODO Tratar array do payload maior que o buffer provisionado
		}
	}
}
//------------------------------------------------------------------------

/**
 *
 * Funcao disponibilizada para aplicacao preencher o payload
 *
 */
void doAnswer(ArrayPayLoad* ans) {

	if (ans) {

		setPayLoad(&dataCom, ans);

		strcpy(dataCom.operacao, OPERATION_AN);
		dataCom.dest	= dataCom.address;
		dataCom.address	= ADDRESS;

		buildFrame();
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

	clearData(&dataCom);
	clearArrayFrame(&frameCom);
	clearBuffer(&bufferRx);
	clearBuffer(&bufferTx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------
