/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */

/* Begin of <includes> initialization, DO NOT MODIFY LINES BELOW */

#include "TSK1.h"
#include "FRTOS1.h"
#include "frtos_tasks.h"

/* End <includes> initialization, DO NOT MODIFY LINES ABOVE */

#include "application.h"
#include "AppQueues.h"
#include "ihm.h"
#include "protocol.h"
#include "Level.h"
#include "accelerometer.h"
#include "gps.h"

/*
 *
 * Main task
 *
 */
static portTASK_FUNCTION(main_task, pvParameters) {

	initQueues();

	initInfo();

	for (;;) {

		App_Run();

//		vTaskDelay(xMainDelay);
	}

	vTaskDelete(main_task);
}

/*
 * Processamento do Protocolo
 *
 */
static portTASK_FUNCTION(communication_task, pvParameters) {

	initCallBacks();

	for (;;) {

		Communication_Run();

		vTaskDelay(xCommunicationDelay);
	}

	vTaskDelete(communication_task);
}

/*
 *
 * Task de leitura dos canais do AD
 */
static portTASK_FUNCTION(data_task, pvParameters) {

	for (;;) {

		Analog_Run();

		vTaskDelay(xDataDelay);
	}

	vTaskDelete(data_task);
}

/**
 *
 * Task para atualizacao do IHM
 *
 */
static portTASK_FUNCTION(ihm_task, pvParameters) {

	ihm_initialize();

	for (;;) {

		IHM_Run();

		vTaskDelay(xIHMDelay);
	}

	ihm_terminate();
	vTaskDelete(ihm_task);
}

/**
 *
 * Task para comunicacao com o GPS
 *
 */
static portTASK_FUNCTION(gps_task, pvParameters) {

	NMEA_init();

	for (;;) {

		NMEA_Run();

		vTaskDelay(xGPSDelay);
	}

	vTaskDelete(gps_task);
}

/**
 *
 * Task leitura do acelerometro
 *
 */
static portTASK_FUNCTION(accel_task, pvParameters) {

	initAccel();

	for (;;) {

		Accelerometer_Run();

		vTaskDelay(xAccelDelay);
	}

	vTaskDelete(accel_task);
}

void CreateTasks(void) {

	if (FRTOS1_xTaskCreate(
			main_task, /* pointer to the task */
			"mains_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleMainTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			communication_task, /* pointer to the task */
			"communication_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleCommunicationTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			data_task, /* pointer to the task */
			"data_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleDataTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			ihm_task, /* pointer to the task */
			"ihm_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleIHMTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}

	if (FRTOS1_xTaskCreate(
			gps_task, /* pointer to the task */
			"gps_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleGPSTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}

	if (FRTOS1_xTaskCreate(
			accel_task, /* pointer to the task */
			"accel_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleAccelTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
}
