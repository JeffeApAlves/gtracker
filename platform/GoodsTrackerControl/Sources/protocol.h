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

// Endereco desse Rastreador
#define	ADDRESS			2

#define CHAR_START		'['
#define CHAR_END		']'
#define CHAR_SEPARATOR	':'
#define CHAR_CR			'\r'
#define CHAR_LF			'\n'
#define CHAR_STR_END	'\0'

static void rxStartCMD (void);
static void receiveFrame (void);
static void rxLF(void);
static void rxCR(void);
static Resource getResource(char* name);
static void sendResult(void);
static void acceptRxFrame();
static void setStatusRx(StatusRx sts);
static void sendString(const char* str);
static void errorRxFrame(void);
static bool decoderFrame(void);
static void verifyFrame(void);
static void errorExec(void);
static void startTX(void);
static void sendFrame(DataCom *frame);
static void setPayLoad(DataCom* frame,ArrayPayLoad* ans);
static void copyHeaderToFrame(void);
static void copyPayLoadToFrame(void);
static void copyCheckSumToFrame(void);
static void buildFrame();

/*interface*/
bool getRxData(char* ch);
bool putTxData(char data);

bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
void doAnswer(ArrayPayLoad* ans);
void initRxCMD(void);
void runCommunication(void);
//void setEventCMD(ResourceID id,pCallBack c);
void anyResult(void);

#endif /* SOURCES_PROTOCOL_H_ */
