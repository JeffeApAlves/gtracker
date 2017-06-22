
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

#include "GT_TSK.h"
#include "GT_FRTOS.h"
#include "frtos_tasks.h"

/* End <includes> initialization, DO NOT MODIFY LINES ABOVE */

#include "application.h"

static portTASK_FUNCTION(main_taskTask, pvParameters) {

  /* Write your task initialization code here ... */

  for(;;) {
    /* Write your task code here ... */

      vTaskDelay(1000/portTICK_RATE_MS);
  }
  /* Destroy the task */
  vTaskDelete(main_taskTask);
}

static portTASK_FUNCTION(IHM_taskTask, pvParameters) {

	ihm_initialize();

	  for(;;) {
		  ihm_loop();
		  vTaskDelay(5/portTICK_RATE_MS);
	  }

	  ihm_terminate();
	  vTaskDelete(IHM_taskTask);
}

static portTASK_FUNCTION(communication_taskTask, pvParameters) {

  initCallBacks();

  for(;;) {
      processProtocol();
      vTaskDelay(5/portTICK_RATE_MS);
  }
  /* Destroy the task */
  vTaskDelete(communication_taskTask);
}

static portTASK_FUNCTION(data_taskTask, pvParameters) {

  initAccel();

  for(;;) {
      read_accel();
      read_Channels_AD();
      vTaskDelay(100/portTICK_RATE_MS);
  }

  vTaskDelete(data_taskTask);
}

void CreateTasks(void) {
  if (GT_FRTOS_xTaskCreate(
     main_taskTask,  /* pointer to the task */
      "main_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }
  if (GT_FRTOS_xTaskCreate(
     IHM_taskTask,  /* pointer to the task */
      "IHM_task", /* task name for kernel awareness debugging */
      configMINIMAL_STACK_SIZE + 0, /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY + 0,  /* initial priority */
      (xTaskHandle*)NULL /* optional task handle to create */
    ) != pdPASS) {
      /*lint -e527 */
      for(;;){}; /* error! probably out of memory */
      /*lint +e527 */
  }
  if (GT_FRTOS_xTaskCreate(
     communication_taskTask,  /* pointer to the task */
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
  if (GT_FRTOS_xTaskCreate(
     data_taskTask,  /* pointer to the task */
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
}
