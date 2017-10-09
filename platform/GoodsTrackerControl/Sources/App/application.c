/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>

#include "FRTOS1.h"

#include "Tank.h"
#include "Telemetria.h"
#include "clock.h"
#include "NMEAFrame.h"
#include "Accelerometer.h"
#include "Serialization.h"
#include "communication.h"
#include "consumer.h"
#include "application.h"

static const char* name_task 	= "task_app";
static const char* name_task_cb = "task_callback";

static 	int		_lock;
Telemetria		telemetria;
TaskHandle_t	xHandleAppTask,xHandleCBTask;

static void createTaskCallBack(pCallBack pxTaskCode,CommunicationPackage* package){

	if (FRTOS1_xTaskCreate(
		pxTaskCode,	name_task_cb,
		configMINIMAL_STACK_SIZE + 50,
		(void*) package,
		tskIDLE_PRIORITY + 10,
		&xHandleCBTask
	) != pdPASS) {
		for (;;) {};
	}
}
//--------------------------------------------------------------------------------------------------------

static void execCMD(uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_RX_FRAME){

		CommunicationPackage	package;

		while (xQueueReceive(xQueuePackageRx, &package, (TickType_t ) 1)) {

			pCallBack cb = getCallBack(package.Header.resource);

			if(cb!=NULL){

				createTaskCallBack(cb,&package);
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

//ResultExec onLED(CommunicationPackage* package){
static portTASK_FUNCTION(onLED, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

//ResultExec onAnalog(CommunicationPackage* package){
static portTASK_FUNCTION(onAnalog, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...


	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

//ResultExec onAccel(CommunicationPackage* package){
static portTASK_FUNCTION(onAccel, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

//ResultExec onTouch(CommunicationPackage* package){
static portTASK_FUNCTION(onTouch, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//-------------------------------------------------------------------------

//ResultExec onPWM(CommunicationPackage* package){
static portTASK_FUNCTION(onPWM, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//------------------------------------------------------------------------

//ResultExec onTelemetry(CommunicationPackage* package){

static portTASK_FUNCTION(onTelemetry, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	initPackageAnswer(package);

	// Preenche payload com resultado
	tlm2String(&telemetria,&package->PayLoad);

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//------------------------------------------------------------------------

//ResultExec onLock(CommunicationPackage* package){
static portTASK_FUNCTION(onLock, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	decoderLockPayLoad(&package->PayLoad);

	if(_lock){

		lock();

	}else{

		unLock();
	}

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	putPackageTx(package);

	vTaskDelete(xHandleCBTask);
}
//------------------------------------------------------------------------

void execError(CommunicationPackage* package){

	initPackageAnswer(package);

	AppendPayLoad(&package->PayLoad,"EXEC ERROR");;
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

		// Clear payload
		clearArrayPayLoad(&package->PayLoad);

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

static void createTask(void){

	if (FRTOS1_xTaskCreate(
		task_app,	name_task,configMINIMAL_STACK_SIZE + 50,
		(void*)NULL,	tskIDLE_PRIORITY + 4,	&xHandleAppTask
	) != pdPASS) {
		for (;;) {};
	}
}
//------------------------------------------------------------------------

void app_init(void){

	createTask();
	clearTelemetria(&telemetria);
}
//------------------------------------------------------------------------
