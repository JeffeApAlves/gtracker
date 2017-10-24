/*
 * Communication.c
 *
 *  Created on: Sep 24, 2017
 *      Author: Jefferson
 */
#include <string.h>

#include "clock.h"
#include "ihm.h"
#include "protocol.h"
#include "communication.h"

/* Task RX*/
static const char*		RX_TASK_NAME =			"tk_rx";
#define					RX_NUM_MSG				1
#define 				RX_TASK_PRIORITY		(tskIDLE_PRIORITY+2)
#define					RX_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 10)
static const TickType_t RX_TASK_DELAY	= 		(200 / portTICK_PERIOD_MS);
static TaskHandle_t 	xHandleRxTask;
QueueHandle_t			xQueuePackageRx;

/* Task TX*/
static const char*		TX_TASK_NAME =			"tk_tx";
#define 				TX_TASK_PRIORITY		(tskIDLE_PRIORITY+2)
#define					TX_NUM_MSG				1
#define					TX_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 140)
static TaskHandle_t 	xHandleTxTask;
QueueHandle_t			xQueuePackageTx;

EventGroupHandle_t		communication_events;

/**
 * Gerencia a fila de pacotes de recepção
 *
 */
static portTASK_FUNCTION(rxPackage_task, pvParameters) {

	while(1) {

		if(receivePackage()){

			// pacote recebido com sucesso e esta na fila
		}

		vTaskDelay(RX_TASK_DELAY);
	}

	vTaskDelete(xHandleRxTask);
}
//---------------------------------------------------------------------------------------------

/**
 * Gerencia a fila de pacotes de transmissão
 *
 */
static portTASK_FUNCTION(txPackage_task, pvParameters) {

	while(1) {

		EventBits_t uxBits	= xEventGroupWaitBits(communication_events,	BIT_TX,pdTRUE,pdFALSE, portMAX_DELAY);

		if(uxBits & BIT_TX){

			CommunicationPackage*	package_tx = pvPortMalloc(sizeof(CommunicationPackage));

			if(package_tx!=NULL){
				while (xQueueReceive(xQueuePackageTx, package_tx, (TickType_t ) 1)) {

					sendPackage(package_tx);
				}

				vPortFree(package_tx);
			}
		}
	}

	vTaskDelete(xHandleTxTask);
}
//---------------------------------------------------------------------------------------------

static void createTasks(void){

	if (xTaskCreate(
			rxPackage_task,
			RX_TASK_NAME,
			RX_TASK_STACK_SIZE,
			(void*)NULL,
			RX_TASK_PRIORITY,
			&xHandleRxTask
	) != pdPASS) {

		while (1) {};
	}

	if (xTaskCreate(
			txPackage_task,
			TX_TASK_NAME,
			TX_TASK_STACK_SIZE,
			(void*)NULL,
			TX_TASK_PRIORITY,
			&xHandleTxTask
	) != pdPASS) {

		while (1) {};
	}
}
//--------------------------------------------------------------------------------------

void communication_init(void){

	protocol_init();

	xQueuePackageRx	= xQueueCreate( RX_NUM_MSG, sizeof( CommunicationPackage ));
	xQueuePackageTx	= xQueueCreate( TX_NUM_MSG, sizeof( CommunicationPackage ));

	communication_events	= xEventGroupCreate();

	createTasks();
}
//------------------------------------------------------------------------------------

void communication_notify_rx(void){

	xEventGroupSetBits(communication_events, BIT_RX);
}
//------------------------------------------------------------------------------------

void communication_notify_tx(void){

	xEventGroupSetBits(communication_events, BIT_TX);
}
//------------------------------------------------------------------------------------

/**
 * Coloca pacote(cmd) recebido na fila e notifica a thread que executa callback
 *
 */
void putPackageRx(CommunicationPackage* package_rx){

	if(xQueueSendToBack( xQueuePackageRx ,package_rx, ( TickType_t ) 1 ) ){

		communication_notify_rx();
	}
}
//------------------------------------------------------------------------------------

/**
 *
 *Coloca pacote(resposta) na fila e avisa a thread responsável pelo envio.
 *
 */

void putPackageTx(CommunicationPackage* package_tx){

	// Publica resposta na fila
	if(xQueueSendToBack( xQueuePackageTx , package_tx, ( TickType_t ) 1 ) ){

		communication_notify_tx();

	}else{

		// Erro ao tentar publicar a resposta
	}
}
//------------------------------------------------------------------------------------

/*
 *
 * Envia resposta
 *
 */
void sendAnswer(CommunicationPackage* package){

	if(package){

		/* Preenche o cabeçalho */
		strcpy(package->Header.operacao,OPERATION_AN);
		package->Header.dest			= package->Header.address;
		package->Header.address			= ADDRESS;
		package->Header.time_stamp		= getCurrentTimeStamp();

		putPackageTx(package);
	}
}
//------------------------------------------------------------------------
