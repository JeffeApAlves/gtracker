/*
 * communication.h
 *
 *  Created on: 21 de set de 2017
 *      Author: jefferson
 */

#ifndef COMPONENTS_COMMUNICATION_INCLUDE_COMMUNICATION_H_
#define COMPONENTS_COMMUNICATION_INCLUDE_COMMUNICATION_H_

#include "rtos.h"
#include "CommunicationFrame.h"
#include "Telemetria.h"

//Endereço do rastreador
#define	TRACKER_ADDRESS	2
#define	HOST_ADDRESS	1

#define	BIT_TX_FRAME	0x01
#define	BIT_RX_FRAME	0x02


void communication_init();
void publishTLM(int address,Telemetria* tlm);
void sendCMD(int tracker_address,Resource cmd,PayLoad* payload);
void putPackageRx(CommunicationPackage* package);

extern QueueHandle_t	xQueueRxPackage,
						xQueueTxPackage;



extern EventGroupHandle_t	communication_event;

#endif /* COMPONENTS_COMMUNICATION_INCLUDE_COMMUNICATION_H_ */
