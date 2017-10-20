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

/**
 * Cria a task de inicialização
 *
 */
void CreateTasks(void) {

	ihm_init();

	tank_init();

	gps_init();

	accelerometer_init();

	clock_init();

	communication_init();

	app_init();
}
//-----------------------------------------------------------------------------------------------
