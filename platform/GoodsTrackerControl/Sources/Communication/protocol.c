#include <stdlib.h>
#include <string.h>

#include "XF1.h"
#include "AS1.h"
#include "Events.h"

#include "Frame.h"
#include "RingBuffer.h"
#include "communication.h"
#include "Serialization.h"
#include "protocol.h"

char SEPARATOR[] = {CHAR_SEPARATOR,CHAR_STR_END};
const char* OPERATION_AN = "AN";
const char* OPERATION_RD = "RD";
const char* OPERATION_WR = "WR";

static StatusRx				statusRx = CMD_INIT;
static RingBuffer			bufferRx,bufferTx;
static Frame				frameRx;

bool processRx(void){

	bool rx_ok = false;

	while(isAnyRxData()){

		switch(statusRx){

			default:
			case CMD_INIT:			protocol_init();		break;
			case CMD_INIT_OK:		rxStartCMD();			break;
			case CMD_RX_START:		receiveFrame();			break;
			case CMD_RX_FRAME:		receiveFrame();			break;
			case CMD_RX_END:		rxCR();					break;
			case CMD_RX_CR:			rxLF();					break;
			case CMD_RX_LF:			rx_ok = verifyFrame();	break;
			case CMD_FRAME_NOK:		errorRxFrame();			break;
		}
	}

	return rx_ok;
}
//------------------------------------------------------------------------

static void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_START){

			clearArrayFrame(&frameRx);
			setStatusRx(CMD_RX_START);
		}
	}
}
//------------------------------------------------------------------------

static void receiveFrame (void) {

	char ch;

	if(getRxData(&ch)) {

		if(ch==CHAR_START || frameRx.Length>=LEN_FRAME) {

			errorRxFrame();
		}
		else
		  if(ch==CHAR_END) {

			 if(frameRx.Length>=SIZE_MIN_FRAME) {

				setStatusRx(CMD_RX_END);
			 }
			 else {

			   errorRxFrame();
			 }
		}
		else {

			putDataArray(&frameRx,ch);
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

			errorRxFrame();
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

			errorRxFrame();
		}
	}
}
//------------------------------------------------------------------------

static bool verifyFrame(void) {

	bool rx_ok = false;

	CommunicationPackage	package_rx;

	if(decoderFrame(&package_rx)){

		acceptRxFrame(&package_rx);

		rx_ok = true;

	}else{

		errorRxFrame();
	}

	return rx_ok;
}
//------------------------------------------------------------------------

static bool decoderFrame(CommunicationPackage* package_rx) {

	bool ret = false;

	clearData(package_rx);

	uint16 count = getNumField(frameRx.Data,CHAR_SEPARATOR);

	// O minimo são 8  itens (7 + checksun)
	if(count >= 8){

		uint16 checksum_rx,checksum_calc;

		//Desconsiderando LEN_CHECKSUM no calculo do checksum que esta no frame recebido
		checksum_calc = calcChecksum(frameRx.Data, frameRx.Length - LEN_CHECKSUM);
		// garante que estao diferentes antes de ler.
		checksum_rx = ~checksum_calc;

		AsHex(&checksum_rx,frameRx.Data,count-1,CHAR_SEPARATOR);

		if(checksum_rx==checksum_calc) {

			if(
				AsInteger(&package_rx->Header.address,		frameRx.Data,0,CHAR_SEPARATOR) &&
				AsInteger(&package_rx->Header.dest,			frameRx.Data,1,CHAR_SEPARATOR) &&
				AsInteger(&package_rx->Header.time_stamp,	frameRx.Data,2,CHAR_SEPARATOR) &&
				AsString(&package_rx->Header.operacao,		frameRx.Data,3,CHAR_SEPARATOR) &&
				AsResource(&package_rx->Header.resource,	frameRx.Data,4,CHAR_SEPARATOR) &&
				AsInteger(&package_rx->Header.lengthPayLoad,frameRx.Data,5,CHAR_SEPARATOR) &&
				AsString(&package_rx->PayLoad.Data,			frameRx.Data,6,CHAR_SEPARATOR)
			){
				package_rx->PayLoad.Length = package_rx->Header.lengthPayLoad;
				ret = true;
			}else{
				clearData(&package_rx);
			}
		}
	}

	return ret;
}
//------------------------------------------------------------------------

Resource getResource(char* name) {

	Resource r;

	if(strcmp(name, 	"LED") == 0)	r = CMD_LED;
	else if(strcmp(name,"LCK") == 0)	r = CMD_LOCK;
	else if(strcmp(name,"TLM") == 0)	r = CMD_TLM;
	else if(strcmp(name,"LCD") == 0)	r = CMD_LCD;
	else if(strcmp(name,"TOU") == 0)	r = CMD_TOUCH;
	else if(strcmp(name,"ACC") == 0)	r = CMD_ACC;
	else if(strcmp(name,"PWM") == 0)	r = CMD_PWM;
	else if(strcmp(name,"ANL") == 0)	r = CMD_ANALOG;
	else								r = CMD_NONE;

	return r;
}
//------------------------------------------------------------------------

void getResourceName(char* out,Resource resource) {

	switch(resource){

		default:
		case CMD_NONE:		strcpy(out,"---");	break;
		case CMD_LOCK:		strcpy(out,"LCK");	break;
		case CMD_TLM:		strcpy(out,"TLM");	break;
		case CMD_LCD:		strcpy(out,"LCD");	break;
		case CMD_TOUCH:		strcpy(out,"TOU");	break;
		case CMD_ACC:		strcpy(out,"ACC");	break;
		case CMD_PWM:		strcpy(out,"PWM");	break;
		case CMD_ANALOG:	strcpy(out,"ANL");	break;
		case CMD_LED:		strcpy(out,"LED");	break;
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
static void acceptRxFrame(CommunicationPackage* package_rx) {

	putPackageRx(package_rx);

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

/**
 * Erro na recepcao do frame
 *
 */
static void errorRxFrame(void){

	setStatusRx(CMD_FRAME_NOK);
	//clearData(&package_rx);
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
 *  Preenche todo o frame  de envio
 *
 */
static void buildFrame(CommunicationPackage* package,Frame* frame) {

	clearArrayFrame(frame);

	copyHeaderToFrame(package,frame);
	copyPayLoadToFrame(package,frame);
	copyCheckSumToFrame(frame);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o cabecalho no frame
 *
 */
static void copyHeaderToFrame(CommunicationPackage* package,Frame* frame) {

	headerToStr(frame->Data,package);
}
//------------------------------------------------------------------------

/**
 *
 * Envia o frame
 *
 */
void sendFrame(char* frame){

	putTxData(CHAR_START);					// Envia caracter de inicio

	putString(&bufferTx,frame);				// Envia o frame

	putTxData(CHAR_END);					// Envia o caracter de fim

	putTxData(CHAR_CR);						// Envia caracteres de controle
	putTxData(CHAR_LF);

	startTX();	// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX
}
//------------------------------------------------------------------------

/*
 *
 * Transmite o frame ao Host
 *
 */
void sendPackage(CommunicationPackage* package){

	char	checksum_str[LEN_CHECKSUM+2],
			header_str[SIZE_HEADER+1];

	unsigned int  checksum;

	package->Header.lengthPayLoad = package->PayLoad.Length;

	header2String(&package->Header,header_str);

	checksum  = calcChecksum (header_str,SIZE_HEADER) ^
				calcChecksum (package->PayLoad.Data,package->PayLoad.Length)^
				CHAR_SEPARATOR;

	XF1_xsprintf(checksum_str, "%c%02X", CHAR_SEPARATOR , checksum);

	putTxData(CHAR_START);						// Envia caracter de inicio

	putString(&bufferTx,header_str);			// Envia header
	putString(&bufferTx,package->PayLoad.Data);	// Envia payload
	putString(&bufferTx,checksum_str);			// Envia checksum

	putTxData(CHAR_END);						// Envia o caracter de fim
	putTxData(CHAR_CR);							// Envia caracteres de controle
	putTxData(CHAR_LF);

	startTX();									// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o Payload no Frame
 *
 */
inline static void copyPayLoadToFrame(CommunicationPackage* package,Frame* frame) {

	AppendFrame(frame,package->PayLoad.Data);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o CheckSum no Frame
 *
 */
static void copyCheckSumToFrame(Frame* frame) {

	AppendFrame(frame,SEPARATOR);
	XF1_xsprintf(frame->checksum, "%02X", calcChecksum (frame->Data,frame->Length));
	AppendFrame(frame,frame->checksum);
}
//------------------------------------------------------------------------

/**
 *
 * Executada quando se recebe uma menssagem na fila de repsotas de comandos
 *
 */
void doAnswer(CommunicationPackage* package) {

	if (package) {

		sendPackage(package);
	}
	else {

		//TODO O QUE RESPONDE EM CASO DE MENSAGEM NULA ???
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

/*
 *
 * Verifica se exsite algum dado no buffer circular de recebimento
 *
 */
inline bool isAnyRxData(){

	return getCount(&bufferRx)>0;
}
//------------------------------------------------------------------------

/**
 *
 * Inicializa a comunicação com o Host via porta serial
 *
 */
void protocol_init(void) {

	clearArrayFrame(&frameRx);
	clearBuffer(&bufferRx);
	clearBuffer(&bufferTx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------