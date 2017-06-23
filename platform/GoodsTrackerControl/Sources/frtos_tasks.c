
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

static portTASK_FUNCTION(mains_taskTask, pvParameters) {

	while(1) {

		vTaskDelay(20/portTICK_RATE_MS);
	}

	vTaskDelete(mains_taskTask);
}
//---------------------------------------------------------------------------------------

static portTASK_FUNCTION(communication_taskTask, pvParameters) {

	initCallBacks();

	while(1) {

		processProtocol();

		vTaskDelay(5/portTICK_RATE_MS);
	}

	vTaskDelete(communication_taskTask);
}
//---------------------------------------------------------------------------------------

static portTASK_FUNCTION(data_taskTask, pvParameters) {

	initAccel();

	while (1) {

		read_accel();

		read_Channels_AD();

		vTaskDelay(10/portTICK_RATE_MS);
	}

	vTaskDelete(data_taskTask);
}
//---------------------------------------------------------------------------------------

void CreateTasks(void) {
	if (FRTOS1_xTaskCreate(
			mains_taskTask, /* pointer to the task */
			"mains_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			communication_taskTask, /* pointer to the task */
			"communication_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			data_taskTask, /* pointer to the task */
			"data_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
}
//---------------------------------------------------------------------------------------
