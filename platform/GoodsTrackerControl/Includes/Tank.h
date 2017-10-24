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
#include "timers.h"

typedef struct{

	uint32_t	Level;
	int			Lock;

} Tank;

void lock(void);
void unLock(void);
void tank_init(void);

extern QueueHandle_t	xQueueTank;

#endif /* SOURCES_LEVEL_H_ */
