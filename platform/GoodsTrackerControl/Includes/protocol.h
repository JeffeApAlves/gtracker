/*
 * protocol.h
 *
 *  Created on: 09/06/2017
 *      Author: Suporte
 */

#ifndef SOURCES_PROTOCOL_H_
#define SOURCES_PROTOCOL_H_

#include <stdbool.h>

#include "Frame.h"
#include "CommunicationFrame.h"
#include "utils.h"
#include "Cmd.h"

//Endereco desse Rastreador
#define	ADDRESS			2

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
	CMD_RX_CR,
	CMD_RX_LF,
	CMD_FRAME_OK,
	CMD_FRAME_NOK,
} StatusRx;

static void rxStartCMD (void);
static void receiveFrame (void);
static void rxLF(void);
static void rxCR(void);
static void acceptRxFrame(CommunicationPackage* package_rx);
static void setStatusRx(StatusRx sts);
static void errorRxFrame(void);
static bool decoderFrame(CommunicationPackage* package_rx);
static bool verifyFrame(void);
static void startTX(void);
static void setPayLoad(PayLoad* ans);
static void copyHeaderToFrame(CommunicationPackage* package,Frame* frame);
static void copyPayLoadToFrame(CommunicationPackage* package,Frame* frame);
static void buildFrame(CommunicationPackage* package,Frame* frame);
static void copyCheckSumToFrame(Frame* frame);


/*interface*/
bool getRxData(char* ch);
bool putTxData(char data);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
void protocol_init(void);
bool isAnyRxData();
Resource getResource(char* name);
void getResourceName(char* name,Resource resource);
void sendFrame(char* frame);
void sendPackage(CommunicationPackage* package);
bool processRx(void);
void doAnswer(CommunicationPackage* ans);

extern const char* OPERATION_AN;
extern const char* OPERATION_RD;
extern const char* OPERATION_WR;

#endif /* SOURCES_PROTOCOL_H_ */