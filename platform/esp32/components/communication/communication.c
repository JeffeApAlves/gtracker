/*
 * communication.c
 *
 *  Created on: 21 de set de 2017
 *      Author: jefferson
 */

#include <stdio.h>
#include <string.h>

#include "Cmd.h"
#include "clock.h"
#include "utils.h"
#include "protocol.h"
#include "serialization.h"
#include "broker.h"
#include "communication.h"

// Fila de frames de transmissão e recepção de frames
QueueHandle_t			xQueueRxPackage,xQueueTxPackage;

// Handles Tasks
TaskHandle_t			xHandleRxTask,xHandleTxTask;

EventGroupHandle_t		communication_event;

CommunicationPackage	answer_frame,cmd_rx;


/**
 * Task para publicação dos pacotes no broker.
 * Os pacotes publicados na fila serão serializado e publicado no broker.
 *
 */
static void rxPackage_task(){

	while(1){

		EventBits_t uxBits = xEventGroupWaitBits(
									communication_event,
									BIT_RX_FRAME,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY );

		if(uxBits & BIT_RX_FRAME){

			CommunicationPackage package;

			while (xQueueReceive(xQueueRxPackage, &package, (TickType_t ) 1)) {

				publishPackage(&package);
			}
		}
	}
}
//------------------------------------------------------------------------------------

/**
 * Task para envios dos frames pela serial.
 * Os pacotes publicados na fila serão serializado e enviado pela serial.
 *
 *
 */
static void txPackage_task(){

	while(1){

		EventBits_t uxBits = xEventGroupWaitBits(
									communication_event,
									BIT_TX_FRAME,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY );

		if(uxBits & BIT_TX_FRAME){


			CommunicationPackage package;

			while (xQueueReceive(xQueueTxPackage, &package, (TickType_t ) 1)) {

				printf("Enviando pacote pela serial\n");
				// Envia pela porta serial
				sendPackage(&package);
			}
		}
	}
}
//------------------------------------------------------------------------------------

void communication_init(){

	communication_event	= xEventGroupCreate();
	xQueueTxPackage		= xQueueCreate( 5, sizeof( CommunicationPackage ));
	xQueueRxPackage		= xQueueCreate( 5, sizeof( CommunicationPackage ));

	xTaskCreate(	rxPackage_task,
					"rxFrame_task",
					8192,
					(void*)NULL,
					5,
					&xHandleRxTask
	);

	xTaskCreate(	txPackage_task,
					"txFrame_task",
					8192,
					(void*)NULL,
					5,
					&xHandleTxTask
	);
}
//------------------------------------------------------------------------------------

void publishTLM(int address,Telemetria* tlm){

	CommunicationPackage	package;

	package.Header.address		= address;
	package.Header.dest			= HOST_ADDRESS;
	package.Header.resource		= CMD_TLM;
	package.Header.time_stamp	= unix_time_in_seconds(0, 0, 15, 19, 9, 2017);
	strcpy(package.Header.operacao,"AN");

	tlm2String(tlm,&package.PayLoad);

	if(xQueueSendToBack( xQueueRxPackage , &package, ( TickType_t ) 1 ) ){

		xEventGroupSetBits(communication_event, BIT_RX_FRAME);
	}
}
//------------------------------------------------------------------------------------

void sendCMD(int tracker_address,Resource cmd,PayLoad* payload){

	CommunicationPackage	package;

	package.Header.address		= HOST_ADDRESS;
	package.Header.dest			= tracker_address;
	package.Header.resource		= cmd;
	package.Header.time_stamp	= unix_time_in_seconds(0, 0, 15, 19, 9, 2017);
	strcpy(package.Header.operacao,OPERATION_WR);

	package.PayLoad				= *payload;

	if(xQueueSendToBack( xQueueTxPackage , &package, ( TickType_t ) 1 ) ){

		xEventGroupSetBits(communication_event, BIT_TX_FRAME);
	}
}
//------------------------------------------------------------------------------------

void putPackageRx(CommunicationPackage* package){

	if(xQueueSendToBack( xQueueRxPackage , package, ( TickType_t ) 1 ) ){

		xEventGroupSetBits(communication_event, BIT_RX_FRAME);
	}
}
//------------------------------------------------------------------------------------
