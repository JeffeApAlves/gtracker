/*
 * Level.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_TANK_H_
#define SOURCES_TANK_H_

#include <stdbool.h>
#include <stdint.h>


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "AD1.h"

typedef struct{

	int		Level;
	int		Lock;

} Tank;

void tank_task(void);
void lock(void);
void unLock(void);
void tank_init(void);

extern bool AD_finished;

extern QueueHandle_t	xQueueTank;

extern TaskHandle_t		xHandleDataTask;

#endif /* SOURCES_LEVEL_H_ */
