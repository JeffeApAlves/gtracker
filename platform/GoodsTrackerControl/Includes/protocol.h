/*
 * protocol.h
 *
 *  Created on: 09/06/2017
 *      Author: Suporte
 */

#ifndef SOURCES_PROTOCOL_H_
#define SOURCES_PROTOCOL_H_

#include <stdbool.h>
#include "utils.h"
#include "DataFrame.h"

//Endereco desse Rastreador
#define	ADDRESS			2

#define	BIT_TX			0x01
#define	BIT_RX			0x02

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
static void acceptRxFrame();
static void setStatusRx(StatusRx sts);
static void sendString(const char* str);
static void errorRxFrame(void);
static bool decoderFrame(void);
static void verifyFrame(void);
static void startTX(void);
static void setPayLoad(PayLoad* ans);
static void doAnswer(DataFrame* ans);
static void copyHeaderToFrame(DataFrame* data,ArrayFrame* frame);
static void copyPayLoadToFrame(DataFrame* data,ArrayFrame* frame);
static void copyCheckSumToFrame(ArrayFrame* frame);
static void buildFrame(DataFrame* data,ArrayFrame* frame);
static void sendFrame(char* frame);
void processTx(void);
void processRx(void);


/*interface*/
bool getRxData(char* ch);
bool putTxData(char data);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
void initCommunication(void);
bool isAnyRxData();
Resource getResource(char* name);
void getResourceName(char* name,Resource resource);

extern const char* OPERATION_AN;
extern const char* OPERATION_RD;
extern const char* OPERATION_WR;

#endif /* SOURCES_PROTOCOL_H_ */
