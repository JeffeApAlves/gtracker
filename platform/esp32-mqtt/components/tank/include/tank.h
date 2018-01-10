/*
 * Level.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_LEVEL_H_
#define SOURCES_LEVEL_H_

#include <stdbool.h>

#include "rtos.h"

typedef struct{

	int		Level;
	int		Lock;

} Tank;

void tank_task(void);
void tank_init(void);
void tank_random_data(void);
void tank_sendResultQueue(void);
bool tank_getdata(void);

extern volatile bool AD_finished;

extern QueueHandle_t	xQueueTank;

#endif /* SOURCES_LEVEL_H_ */
