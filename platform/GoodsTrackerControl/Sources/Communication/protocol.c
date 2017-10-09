#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"
#include "communication.h"
#include "serialization.h"
#include "serial.h"
#include "protocol.h"

const char* OPERATION_AN = "AN";
const char* OPERATION_RD = "RD";
const char* OPERATION_WR = "WR";

static StatusRx	statusRx = CMD_INIT;
static Frame	frameRx;

static inline void setStatusRx(StatusRx sts) {

	statusRx = sts;
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
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

static void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_START){

			clearFrame(&frameRx);
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

static bool decoderFrame(CommunicationPackage* package_rx) {

	bool ret = false;

	uint16_t count = getNumField(frameRx.Data,CHAR_SEPARATOR);

	// O minimo são 8  itens (7 + checksun)
	if(count >= 8){

		uint16_t checksum_rx,checksum_calc;

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
				AsString(package_rx->Header.operacao,		frameRx.Data,3,CHAR_SEPARATOR) &&
				AsResource(&package_rx->Header.resource,	frameRx.Data,4,CHAR_SEPARATOR) &&
				AsInteger(&package_rx->Header.lengthPayLoad,frameRx.Data,5,CHAR_SEPARATOR)
			){
				subString(package_rx->PayLoad.Data,frameRx.Data,6,count-1,CHAR_SEPARATOR);
				package_rx->PayLoad.Length = package_rx->Header.lengthPayLoad;

				ret = true;
			}else{
				clearPackage(&package_rx);
			}
		}else{
			clearPackage(package_rx);
		}
	}

	return ret;
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

bool receivePackage(void){

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

/**
 *
 * Envia o frame
 *
 */
void sendFrame(char* frame){

	putTxData(CHAR_START);					// Envia caracter de inicio
	putTxString(frame);						// Envia o frame
	putTxData(CHAR_END);					// Envia o caracter de fim
	putTxData(CHAR_CR);						// Envia caracteres de controle
	putTxData(CHAR_LF);

	startTX();								// Envia 1 byte para iniciar a transmissao. Os demais serao via interrupcao TX
}
//------------------------------------------------------------------------

/*
 *
 * Faz a serialização reaproveitando o payload da camada aplicação e transmite via serial
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

	checkSum2String(checksum,checksum_str);

	putTxData(CHAR_START);						// Envia caracter de inicio
	putTxString(header_str);					// Envia header
	putTxString(package->PayLoad.Data);			// Envia payload
	putTxString(checksum_str);					// Envia checksum
	putTxData(CHAR_END);						// Envia o caracter de fim
	putTxData(CHAR_CR);							// Envia caracteres de controle
	putTxData(CHAR_LF);

	startTX();									// Envia 1 byte para iniciar a transmissao. Os demais serao via interrupcao TX
}
//------------------------------------------------------------------------

/**
 *
 * Inicializa p protocolo de comunicação
 *
 */
void protocol_init(void) {

	clearFrame(&frameRx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------
