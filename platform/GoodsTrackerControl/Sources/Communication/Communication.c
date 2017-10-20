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

/* Task RX*/
static const char*		RX_TASK_NAME =			"task_rx";
#define 				RX_TASK_PRIORITY		(tskIDLE_PRIORITY+1)
#define					RX_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE)
static const TickType_t RX_TASK_DELAY	= 		(200 / portTICK_PERIOD_MS);
QueueHandle_t			xQueuePackageRx;
TaskHandle_t 			xHandleRxTask;

/* Task TX*/
static const char*		TX_TASK_NAME =			"task_tx";
#define 				TX_TASK_PRIORITY		(tskIDLE_PRIORITY+1)
#define					TX_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 150)
TaskHandle_t 			xHandleTxTask;
QueueHandle_t			xQueuePackageTx;

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

		uint32_t ulNotifiedValue;

		xTaskNotifyWait( 0x0, BIT_TX,  &ulNotifiedValue, portMAX_DELAY );

		if(ulNotifiedValue & BIT_TX){

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

	if (FRTOS1_xTaskCreate(
			rxPackage_task,
			RX_TASK_NAME,
			RX_TASK_STACK_SIZE,
			(void*)NULL,
			RX_TASK_PRIORITY,
			&xHandleRxTask
	) != pdPASS) {

		while (1) {};
	}

	if (FRTOS1_xTaskCreate(
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

	xQueuePackageRx	= xQueueCreate( 1, sizeof( CommunicationPackage ));
	xQueuePackageTx	= xQueueCreate( 1, sizeof( CommunicationPackage ));

	createTasks();
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

/*
 *
 * Envia resposta
 *
 */
void sendAnswer(CommunicationPackage* package){

	if(package){

		strcpy(package->Header.operacao,OPERATION_AN);
		package->Header.dest			= package->Header.address;
		package->Header.address			= ADDRESS;
		package->Header.time_stamp		= getCurrentTimeStamp();

		putPackageTx(package);
	}
}
//------------------------------------------------------------------------
