/*
 * protocol.h
 *
 *  Created on: 09/06/2017
 *      Author: Suporte
 */

#ifndef SOURCES_PROTOCOL_H_
#define SOURCES_PROTOCOL_H_

#include "PE_Types.h"
#include "utils.h"
#include "DataFrame.h"

    /**
     *
     * Frame Coomunication
     * [ End. de orig[5] : End dest[5] :  COUNT[5] :Operacao[2] : Recurso[3] : SizePayload[3] : payload[ 0 ~ 255] : CheckSum[2] ] \r\n
     *
     * End. de orig:
     * Range: 00000~65535 (00000) Broadcast
     *
     * End. de dest:
     * Range: 00000~65535 (00000) Broadcast
     *
     * Operacao:
     * Possiveis:
     * RD = READ
     * WR = WRITE
     * AN + ANSWER
     *
     * Recurso:
     * Range: A-Z a-z 0~9
     *
     * SizePayload:
     * Range: 0~255
     *
     * Payload:
     * Conteudo App
     * Observacao: '[' ']' sao caracteres especiais entao usar \] e \[
     *
     * CheckSum
     * Somatoria
     */

#define	ADDRESS			2

#define TIME_TX			5
#define TIME_RX			5

#define CHAR_START		'['
#define CHAR_END		']'
#define CHAR_SEPARATOR	':'
#define CHAR_CR			'\r'
#define CHAR_LF			'\n'
#define CHAR_STR_END	'\0'

/*
 *
 *Retorno das callbacks
 */
typedef enum {

	CMD_RESULT_EXEC_UNSUCCESS	= -3,
	CMD_RESULT_INVALID_CMD		= -2,
	CMD_RESULT_INVALID_PARAM	= -1,
	CMD_RESULT_EXEC_SUCCESS		= 0,

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
	CMD_RX_FRAME,
	CMD_RX_END,
	CMD_RX_NL,
	CMD_RX_CR,
	CMD_FRAME_OK,
	CMD_FRAME_NOK,
	CMD_EXEC,
	CMD_EXEC_ERROR,
} StatusRx;


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

static void rxStartCMD (void);
static void receiveFrame (void);
static void rxNL(void);
static void rxCR(void);
static pCallBack getCallBack(void);
void initRxCMD(void);
static void sendResult(void);
static void acceptRxFrame();
static void setStatusRx(StatusRx sts);
bool getRxData(char* ch);
bool putTxData(char data);
static void sendString(const char* str);
static void errorRxFrame(void);
static bool decoderFrame(void);
static void verifyFrame(void);
static void errorExec(void);
static void startTX(void);
static void sendFrame(DataFrame *frame);
static void setPayLoad(DataFrame* frame, char* str);
static void AppendHeader(DataFrame *frame);
static void AppendPayLoad(DataFrame *frame);
static void AppendCheckSum(DataFrame *frame);
static void buildFrame(DataFrame *frame);

/*interface*/
void processProtocol(void);
void setEventCMD(ResourceID id,pCallBack c);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
void doAnswer(char *msg);

extern unsigned int timeTx,timeRx;

#endif /* SOURCES_PROTOCOL_H_ */
