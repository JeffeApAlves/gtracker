/*
 * Communication.c
 *
 *  Created on: Sep 24, 2017
 *      Author: Jefferson
 */

#include "FRTOS1.h"

#include "ihm.h"
#include "protocol.h"
#include "application.h"
#include "communication.h"

static const TickType_t xRxComDelay		= (200 / portTICK_PERIOD_MS);
static const char* name_task_rx 		= "task_rx";
static const char* name_task_tx 		= "task_tx";

// Handle das tasks para processamento das pilha de TX e RX
TaskHandle_t 	xHandleRxTask,xHandleTxTask;

// Handles das Queues
QueueHandle_t	xQueuePackageRx, xQueuePackageTx;


/**
 * Gerencia a fila de pacotes de recepção
 *
 */
static portTASK_FUNCTION(rxPackage_task, pvParameters) {

	while(1) {

		if(receivePackage()){

			// pacote recebido com sucesso e esta na fila
		}

		vTaskDelay(xRxComDelay);
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

		uint32_t ulNotifiedValue;

		xTaskNotifyWait( 0x0, BIT_TX,  &ulNotifiedValue, portMAX_DELAY );

		if(ulNotifiedValue & BIT_TX){

			CommunicationPackage	package_tx;

			while (xQueueReceive(xQueuePackageTx, &package_tx, (TickType_t ) 1)) {

				sendPackage(&package_tx);
			}
		}
	}

	vTaskDelete(xHandleTxTask);
}
//---------------------------------------------------------------------------------------------

static void createTasks(void){

	if (FRTOS1_xTaskCreate(
			rxPackage_task,
			name_task_rx,
			configMINIMAL_STACK_SIZE+50,
			(void*)NULL,
			tskIDLE_PRIORITY + 1,
			&xHandleRxTask
	) != pdPASS) {

		for (;;) {};
	}

	if (FRTOS1_xTaskCreate(
			txPackage_task,
			name_task_tx,
			configMINIMAL_STACK_SIZE+50,
			(void*)NULL,
			tskIDLE_PRIORITY + 1,
			&xHandleTxTask
	) != pdPASS) {

		for (;;) {};
	}

}
//--------------------------------------------------------------------------------------

void communication_init(void){

	createTasks();

	xQueuePackageRx	= xQueueCreate( 1, sizeof( CommunicationPackage ));
	xQueuePackageTx	= xQueueCreate( 1, sizeof( CommunicationPackage ));

	protocol_init();
}
//------------------------------------------------------------------------------------

/**
 * Coloca pacote(cmd) recebido na fila e notifica a thread que executa callback
 *
 */
void putPackageRx(CommunicationPackage* package_rx){

	if(xQueueSendToBack( xQueuePackageRx ,package_rx, ( TickType_t ) 1 ) ){

		xTaskNotify( xHandleAppTask , BIT_RX_FRAME , eSetBits );
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

		xTaskNotify( xHandleTxTask , BIT_TX , eSetBits );

	}else{

		// Erro ao tentar publicar a resposta
	}
}
//------------------------------------------------------------------------------------
