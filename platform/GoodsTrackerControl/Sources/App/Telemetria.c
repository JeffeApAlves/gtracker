/*
 * Telemetria.c
 *
 *  Created on: Oct 23, 2017
 *      Author: Jefferson
 */
#include "clock.h"
#include "ihm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "Telemetria.h"

/* Task TLM */
static const char*			TLM_TASK_NAME =			"tk_tlm";
#define 					TLM_TASK_PRIORITY		(tskIDLE_PRIORITY+4)
#define						TLM_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE+20)
static TaskHandle_t			xHandleTLMTask;
static EventGroupHandle_t	tlm_events;

Telemetria					telemetria;


static void updateTLM(Telemetria* tlm,EventBits_t ulNotifiedValue);


/**
 *  Task para execucao da call back dos cmds recebidos e consume as informações de telemetria
 *  O comando de telemetria espera as notificacoes para atualizacoes das informacoes via fila de mensagens
 *
 */
static portTASK_FUNCTION(task_tlm, pvParameters) {

	while(1) {

		EventBits_t uxBits	= xEventGroupWaitBits(tlm_events,BIT_UPDATE_GPS | BIT_UPDATE_ACCELEROMETER | BIT_UPDATE_TANK,pdTRUE,pdFALSE, portMAX_DELAY);

		updateTLM(&telemetria,uxBits);
	}

	vTaskDelete(xHandleTLMTask);
}
//--------------------------------------------------------------------------------------------------------

void tlm_notify_accelerometer(void){

	xEventGroupSetBits(tlm_events, BIT_UPDATE_ACCELEROMETER);
}
//-----------------------------------------------------------------------------------

void tlm_notify_tank(void){

	xEventGroupSetBits(tlm_events, BIT_UPDATE_TANK);
}
//-----------------------------------------------------------------------------------

void tlm_notify_gps(void){

	xEventGroupSetBits(tlm_events, BIT_UPDATE_GPS);
}
//-----------------------------------------------------------------------------------

static void updateGPS(Telemetria* tlm) {

	if (xQueueReceive(xQueueGPS, &tlm->GPS, (TickType_t ) 1)) {

		ihm_notify_screen_tlm();

		// Configura o relógio através do GPS somente após indicar dados OK
		if(tlm->GPS.FixQuality>0){
			adjusteClock(tlm->GPS.Date,tlm->GPS.Time_UTC);
		}
	}
}
//-------------------------------------------------------------------------

static void updateAccelerometer(Telemetria* tlm) {

	if (xQueueReceive(xQueueAcce, &tlm->Accelerometer, (TickType_t ) 1)) {

		ihm_notify_screen_tlm();
	}
}
//-------------------------------------------------------------------------

static void updateTankLevel(Telemetria* tlm) {

	if (xQueueReceive(xQueueTank, &tlm->Tank, (TickType_t ) 1)) {

		ihm_notify_screen_tlm();
	}
}
//-------------------------------------------------------------------------

/**
 *
 * Atualiza as informções de telemetria que estão na lista de menssagens
 *
 */
static void updateTLM(Telemetria* tlm,EventBits_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_UPDATE_GPS){

		updateGPS(tlm);
	}

	if(ulNotifiedValue & BIT_UPDATE_ACCELEROMETER){

		updateAccelerometer(tlm);
	}

	if(ulNotifiedValue & BIT_UPDATE_TANK){

		updateTankLevel(tlm);
	}
}
//-----------------------------------------------------------------------------------

static void createTask(void){

	if (xTaskCreate(
		task_tlm,
		TLM_TASK_NAME,
		TLM_TASK_STACK_SIZE,
		(void*)NULL,
		TLM_TASK_PRIORITY,
		&xHandleTLMTask
	) != pdPASS) {

	}
}
//------------------------------------------------------------------------

void tlm_init(void){

	clearTelemetria(&telemetria);

	tlm_events	= xEventGroupCreate();

	createTask();
}
//-----------------------------------------------------------------------------------



