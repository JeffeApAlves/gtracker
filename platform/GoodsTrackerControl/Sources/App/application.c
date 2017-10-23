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
#include "application.h"

/* Task CB (cmd) */
static const char*	CB_TASK_NAME =			"tk_callback";
#define 			CB_TASK_PRIORITY		(configMAX_PRIORITIES)
#define				CB_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 150)
TaskHandle_t		xHandleCBTask;

static 	int		_lock;

static void createTaskCallBack(pCallBack pxTaskCode,CommunicationPackage* package);
static void execError(CommunicationPackage* package);
static void execCMD(EventBits_t ulNotifiedValue);

static portTASK_FUNCTION(task_cb, pvParameters) {

	while(1) {

		EventBits_t uxBits	= xEventGroupWaitBits(communication_events,BIT_RX,pdTRUE,pdFALSE, portMAX_DELAY);

		if(uxBits & BIT_RX){

			execCMD(uxBits);
		}
	}

	vTaskDelete(xHandleCBTask);
}
//--------------------------------------------------------------------------------------------------------

static void execCMD(EventBits_t ulNotifiedValue){

	CommunicationPackage*	package = pvPortMalloc(sizeof(CommunicationPackage));

	if(package!=NULL){

		while (xQueueReceive(xQueuePackageRx, package, (TickType_t ) 1)) {

			pCallBack cb = getCallBack(package->Header.resource);

			if(cb==NULL || (!cb(package))){

				execError(package);
			}

			vPortFree(package);
		}
	}
}
//-------------------------------------------------------------------------

static HOOK_CMD(onLED, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	// Resultado da callback
	sendAnswer(package);

	//vPortFree(package);
	//vTaskDelete(NULL);
	return true;
}
//-------------------------------------------------------------------------

static HOOK_CMD(onAnalog, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	//vPortFree(package);
	//vTaskDelete(NULL);
	return true;
}
//-------------------------------------------------------------------------

static HOOK_CMD(onAccel, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	// Resultado da callback
	sendAnswer(package);

	//vPortFree(package);
	//vTaskDelete(NULL);
	return true;
}
//-------------------------------------------------------------------------

static HOOK_CMD(onTouch, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	//vPortFree(package);
	//vTaskDelete(NULL);
	return true;
}
//-------------------------------------------------------------------------

static HOOK_CMD(onPWM, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	// Resultado da callback
	sendAnswer(package);

	//vPortFree(package);
	//vTaskDelete(NULL);
	return true;
}
//------------------------------------------------------------------------

static HOOK_CMD(onTelemetry, pvParameters){

	CommunicationPackage* package = (CommunicationPackage*)pvParameters;

	// ... Implementação da execução do cmd ...
	// Preenche payload com resultado
	tlm2String(&telemetria,&package->PayLoad);

	// Resultado da callback
	sendAnswer(package);

	//vPortFree(package);
	//vTaskDelete(NULL);
	return true;
}
//------------------------------------------------------------------------

static HOOK_CMD(onLock, pvParameters){

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

	//vPortFree(package);
	//vTaskDelete(NULL);

	return true;
}
//------------------------------------------------------------------------

static void execError(CommunicationPackage* package){

	AppendPayLoad(&package->PayLoad,"EXEC ERROR");

	sendAnswer(package);

	//vPortFree(package);
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

static void createTask(void){

	if (xTaskCreate(
		task_cb,
		CB_TASK_NAME,
		CB_TASK_STACK_SIZE,
		(void*) NULL,
		CB_TASK_PRIORITY,
		&xHandleCBTask
	) != pdPASS) {
		while (1) {};
	}
}
//------------------------------------------------------------------------

void app_init(void){

	createTask();
}
//------------------------------------------------------------------------
