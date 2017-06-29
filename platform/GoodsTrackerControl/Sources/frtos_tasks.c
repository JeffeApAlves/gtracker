
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
#include "gps.h"

const TickType_t xMainDelay 			= 50 / portTICK_PERIOD_MS;
const TickType_t xCommunicationDelay	= 5 / portTICK_PERIOD_MS;
const TickType_t xDataDelay				= 10 / portTICK_PERIOD_MS;
const TickType_t xIHMDelay				= 25 / portTICK_PERIOD_MS;
const TickType_t xGPSDelay				= 5 / portTICK_PERIOD_MS;

/*
 *
 * Main task
 *
 */
static portTASK_FUNCTION(main_task, pvParameters) {

	initQueues();

	for(;;) {

		updateDataTLM();

		vTaskDelay(xMainDelay);
	}

	vTaskDelete(main_task);
}


/*
 * Processamento do Protocolo
 *
 */
static portTASK_FUNCTION(communication_task, pvParameters) {

  initCallBacks();

  for(;;) {

		processProtocol();

		vTaskDelay(xCommunicationDelay);
  }

  vTaskDelete(communication_task);
}

/*
 *
 * Task de aquisicao do acelerometro e canais AD
 */
static portTASK_FUNCTION(data_task, pvParameters) {

  initAccel();

  for(;;) {

	  read_accel();
	  read_Channels_AD();
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

	for(;;) {

		ihm_loop();
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

	for(;;) {

		NMEA_process();
		vTaskDelay(xGPSDelay);
	}

	vTaskDelete(gps_task);
}

void CreateTasks(void) {
  if (FRTOS1_xTaskCreate(
     main_task,  /* pointer to the task */
      "mains_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }
  if (FRTOS1_xTaskCreate(
     communication_task,  /* pointer to the task */
      "communication_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }
  if (FRTOS1_xTaskCreate(
     data_task,  /* pointer to the task */
      "data_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }
  if (FRTOS1_xTaskCreate(
     ihm_task,  /* pointer to the task */
      "ihm_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }

  if (FRTOS1_xTaskCreate(
     gps_task,  /* pointer to the task */
      "gps_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }
}

