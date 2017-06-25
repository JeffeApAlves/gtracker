/*
 * protocol.h
 *
 *  Created on: 09/06/2017
 *      Author: Suporte
 */

#ifndef SOURCES_PROTOCOL_H_
#define SOURCES_PROTOCOL_H_

#include "PE_Types.h"

#define	ADDRESS			2

#define TIME_TX			5
#define TIME_RX			5

#define LEN_ADDRESS		5
#define LEN_ORIGEM		5
#define LEN_OPERATION	2
#define LEN_RESOURCE	3
#define LEN_SIZE_PL		3
#define LEN_CHECKSUM	2
#define LEN_HEADER		(LEN_ADDRESS+LEN_ORIGEM+LEN_OPERATION+LEN_RESOURCE+LEN_SIZE_PL+4) // 4 separadores do cabecalho

#define SIZE_MIN_FRAME		(LEN_HEADER+2)
#define SIZE_MAX_PAYLOAD	255
#define SIZE_MAX_FRAME		(LEN_HEADER+SIZE_MAX_PAYLOAD+2)


/* [LED01]\r\n*/
#define CHAR_CMD_START	'['
#define CHAR_CMD_END	']'
#define CHAR_CR			'\r'
#define CHAR_LF			'\n'
#define CHAR_SEPARATOR	':'
#define CHAR_NAK		0x15
#define CHAR_STR_END	'\0'

/*
 *
 *Retorno das callbacks
 */
typedef enum {

	CMD_RESULT_EXEC_UNSUCCESS	= -3,
	CMD_RESULT_INVALID_CMD		= -2,
	CMD_RESULT_INVALID_PARAM	= -1,
	CMD_RESULT_EXEC_SUCCESS	= 0,

}ResultExec;

/*
 *
 * Id dos recursos
 */
typedef enum {

	CMD_LED,
	CMD_ANALOG,
	CMD_PWM,
	CMD_TOUCH,
	CMD_ACC,
	CMD_TELEMETRIA,
	CMD_LOCK,
	CMD_LCD

}ResourceID;

/**
 *
 * Maquina de estado para recebimento do frame
 */
typedef enum {

	CMD_INIT,
	CMD_INIT_OK,
	CMD_RX_START,
	CMD_RX_PAYLOAD,
	CMD_RX_END,
	CMD_RX_NL,
	CMD_RX_CR,
	CMD_DECODER,
	CMD_ERROR,
	CMD_EXEC,
	CMD_EXEC_ERROR,
} StatusRx;

/*
 * Estrutura de dados do frame
 *
 */
typedef struct{

	unsigned char	checksum_rx;

	char	operacao[LEN_OPERATION + 1];
	char	resource[LEN_RESOURCE + 1];

	int		address;
	int		dest;

	int		sizeHeader;
	int		sizePayLoad;
	int		sizeFrame;

	char	payload[SIZE_MAX_PAYLOAD];
	char	frame[SIZE_MAX_FRAME];

} DataFrame;


/**
 * Ponteiro para as call backs
 */
typedef ResultExec(*pCallBack)(DataFrame*);


/**
 *
 */
typedef struct{

	ResourceID	resourceID;
	char		resource[LEN_RESOURCE + 1];
	pCallBack	cb;

} Resource;

void rxStartCMD (void);
void receiveFrame (void);
void rxNL(void);
void rxCR(void);
pCallBack getCallBack(void);
void initRxCMD(void);
void sendResult(void);
void execCallBack();
void setStatusRx(StatusRx sts);
bool getRxData(char* ch);
bool putTxData(char data);
void sendString(const char* str);
void clearData(DataFrame* frame);
void errorRx(void);
void decoderFrame(void);
bool decoderFrame2(void);
void errorExec(void);
unsigned char calcChecksum(const char *buff, size_t sz);
void startTX(void);
void setPayLoad(DataFrame* frame, char* str);
void buildHeader(DataFrame *frame);
void buildFrame(DataFrame *frame);

/*interface*/
void processProtocol(void);
void setEventCMD(ResourceID id,pCallBack c);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
void doAnswer(char *msg);

extern unsigned int timeTx,timeRx;

#endif /* SOURCES_PROTOCOL_H_ */
