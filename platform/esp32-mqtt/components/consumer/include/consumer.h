/*
 * consumer.h
 *
 *  Created on: 22 de set de 2017
 *      Author: jefferson
 */

#ifndef COMPONENTS_CONSUMER_INCLUDE_CONSUMER_H_
#define COMPONENTS_CONSUMER_INCLUDE_CONSUMER_H_

#include "rtos.h"

#define	BIT_UPDATE_GPS	0x01
#define	BIT_UPDATE_AD	0x02
#define	BIT_UPDATE_ACCE	0x04

void consumer_init(void);

extern TaskHandle_t			xHandleConsumer;
extern EventGroupHandle_t	consumer_event;

#endif /* COMPONENTS_CONSUMER_INCLUDE_CONSUMER_H_ */
