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
#include "ihm.h"
#include "clock.h"
#include "serial.h"
#include "communication.h"
#include "accelerometer.h"
#include "gps.h"

TaskHandle_t xHandleMainTask;
static const char* name_task			= "task_startup";
static const TickType_t xMainDelay		= (200 / portTICK_PERIOD_MS);

/**
 * Task responsável por fazer o startup ddo sistema e monitora o teclado.
 *
 */
static portTASK_FUNCTION(run_main, pvParameters) {

	tank_init();

	ihm_init();

	gps_init();

	accelerometer_init();

	clock_init();

	uart_init();

	communication_init();

	app_init();

	while(1) {

		ihm_notify_screen_stat();

		readKey();

		vTaskDelay(xMainDelay);
	}

	vTaskDelete(xHandleMainTask);
}
//-----------------------------------------------------------------------------------------------

/**
 * Cria a task de inicialização
 *
 *
 */
void CreateTasks(void) {

	if (FRTOS1_xTaskCreate(
			run_main, /* pointer to the task */
			name_task, /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 10, /* initial priority */
			&xHandleMainTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
}
//-----------------------------------------------------------------------------------------------
