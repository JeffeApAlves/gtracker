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


/*interface*/
void protocol_init(void);
Resource getResource(char* name);
void sendFrame(char* frame);
void sendPackage(CommunicationPackage* package);
bool receivePackage(void);

extern const char* OPERATION_AN;
extern const char* OPERATION_RD;
extern const char* OPERATION_WR;

#endif /* SOURCES_PROTOCOL_H_ */
