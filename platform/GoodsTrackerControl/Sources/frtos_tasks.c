#include "TSK1.h"
#include "FRTOS1.h"
#include "frtos_tasks.h"

#include "application.h"
#include "ihm.h"
#include "clock.h"
#include "serial.h"
#include "communication.h"
#include "accelerometer.h"
#include "gps.h"

TaskHandle_t xHandleMainTask;
static const char* name_task =			"task_startup";
static const TickType_t xMainDelay =	(200 / portTICK_PERIOD_MS);

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
 */
void CreateTasks(void) {

	if (FRTOS1_xTaskCreate(
			run_main,
			name_task,
			configMINIMAL_STACK_SIZE,
			(void*)NULL,
			tskIDLE_PRIORITY + 10,
			&xHandleMainTask
	) != pdPASS) {

		while(1) {};
	}
}
//-----------------------------------------------------------------------------------------------
