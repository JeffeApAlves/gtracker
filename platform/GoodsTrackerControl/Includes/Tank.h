/*
 * Level.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_TANK_H_
#define SOURCES_TANK_H_

#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "timers.h"

typedef struct{

	uint32_t	Level;
	int			Lock;

} Tank;

#define		BIT_AD_VALUE				0x08

void lock(void);
void unLock(void);
void tank_init(void);

BaseType_t tank_notify_value(BaseType_t *xHigherPriorityTaskWoken);

extern QueueHandle_t	xQueueTank;

#endif /* SOURCES_LEVEL_H_ */
