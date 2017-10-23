/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>

#include "Tank.h"
#include "Telemetria.h"
#include "clock.h"
#include "Accelerometer.h"
#include "Serialization.h"
#include "communication.h"
#include "consumer.h"
#include "application.h"

/* Task APP */
static const char*	APP_TASK_NAME =			"tk_app";
#define 			APP_TASK_PRIORITY		(tskIDLE_PRIORITY+5)
#define				APP_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 150)
TaskHandle_t		xHandleAppTask;

/* Task CB (cmd) */
static const char*	CB_TASK_NAME =			"tk_callback";
#define 			CB_TASK_PRIORITY		(configMAX_PRIORITIES)
#define				CB_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 150)
TaskHandle_t		xHandleCBTask;

static 	int		_lock;
Telemetria		telemetria;

static void createTaskCallBack(pCallBack pxTaskCode,CommunicationPackage* package);
static void execError(CommunicationPackage* package);

static void execCMD(uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_RX_FRAME){

		CommunicationPackage*	package = pvPortMalloc(sizeof(CommunicationPackage));

		if(package!=NULL){

			while (xQueueReceive(xQueuePackageRx, package, (TickType_t ) 1)) {

				pCallBack cb = getCallBack(package->Header.resource);

				if(cb!=NULL){

					createTaskCallBack(cb,package);

				} else{

					execError(package);
				}
			}
		}
	}
}
//-------------------------------------------------------------------------

/**
 *  Task para execucao da call back dos cmds recebidos e consume as informações de telemetria
 *  O comando de telemetria espera as notificacoes para atualizacoes das informacoes via fila de mensagens
 *
 */
static portTASK_FUNCTION(task_app, pvParameters) {

	while(1) {

		uint32_t ulNotifiedValue;

		xTaskNotifyWait( 0x0, BIT_RX_FRAME | BIT_UPDATE_GPS | BIT_UPDATE_ACCE | BIT_UPDATE_AD,  &ulNotifiedValue, portMAX_DELAY);

		updateTLM(&telemetria,ulNotifiedValue);

		execCMD(ulNotifiedValue);
	}

	vTaskDelete(xHandleAppTask);
}
//--------------------------------------------------------------------------------------------------------

static portTASK_FUNCTION(onLED, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

static portTASK_FUNCTION(onAnalog, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

static portTASK_FUNCTION(onAccel, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

static portTASK_FUNCTION(onTouch, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

static portTASK_FUNCTION(onPWM, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//------------------------------------------------------------------------

static portTASK_FUNCTION(onTelemetry, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...
	// Preenche payload com resultado
	tlm2String(&telemetria,&package->PayLoad);

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//------------------------------------------------------------------------

static portTASK_FUNCTION(onLock, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	decoderLockPayLoad(&package->PayLoad);

	if(_lock){

		lock();

	}else{

		unLock();
	}

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	vPortFree(package);
	vTaskDelete(xHandleCBTask);
}
//------------------------------------------------------------------------

static void execError(CommunicationPackage* package){

	AppendPayLoad(&package->PayLoad,"EXEC ERROR");

	sendAnswer(package);

	vPortFree(package);
}
//------------------------------------------------------------------------

void decoderLockPayLoad(PayLoad* payload){

	AsInteger(&_lock,payload->Data,0,CHAR_SEPARATOR);
}
//------------------------------------------------------------------------

/*
 *
 * Set endereco de origem e destino e o tipo da operacao
 *
 */
static void initPackageAnswer(CommunicationPackage* package){

	if(package){

		strcpy(package->Header.operacao,OPERATION_AN);
		package->Header.dest			= package->Header.address;
		package->Header.address			= ADDRESS;
		package->Header.time_stamp		= getCurrentTimeStamp();
	}
}
//------------------------------------------------------------------------

pCallBack getCallBack(Resource r) {

	pCallBack	cb = NULL;

	switch(r){

		case CMD_LED:	cb = onLED;			break;
		case CMD_ANALOG:cb = onAnalog;		break;
		case CMD_PWM:	cb = onPWM;			break;
		case CMD_ACC:	cb = onAccel;		break;
		case CMD_TOUCH:	cb = onTouch;		break;
		case CMD_TLM:	cb = onTelemetry;	break;
		case CMD_LOCK:	cb = onLock;		break;
		case CMD_LCD:	cb = NULL;			break;
	}

	return cb;
}
//------------------------------------------------------------------------

static void createTaskCallBack(pCallBack pxTaskCode,CommunicationPackage* package){

	if (xTaskCreate(
		pxTaskCode,
		CB_TASK_NAME,
		CB_TASK_STACK_SIZE,
		(void*) package,
		CB_TASK_PRIORITY,
		&xHandleCBTask
	) != pdPASS) {
		while (1) {};
	}
}
//--------------------------------------------------------------------------------------------------------

static void createTask(void){

	if (xTaskCreate(
		task_app,
		APP_TASK_NAME,
		APP_TASK_STACK_SIZE,
		(void*)NULL,
		APP_TASK_PRIORITY,
		&xHandleAppTask
	) != pdPASS) {
		while(1){};
	}
}
//------------------------------------------------------------------------

void app_init(void){

	clearTelemetria(&telemetria);
	createTask();
}
//------------------------------------------------------------------------
