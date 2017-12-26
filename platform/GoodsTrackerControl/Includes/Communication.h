/*
 * Communication.h
 *
 *  Created on: Sep 24, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_COMMUNICATION_H_
#define INCLUDES_COMMUNICATION_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include "CommunicationFrame.h"

#define	BIT_TX			0x01
#define	BIT_RX			0x02

void putPackageRx(CommunicationPackage* package_rx);
void putPackageTx(CommunicationPackage* package_tx);
void sendAnswer(CommunicationPackage* package);

void communication_init(void);
BaseType_t communication_notify_rx_char(BaseType_t *xHigherPriorityTaskWoken);
//void communication_notify_rx(void);
void communication_notify_tx(void);

extern QueueHandle_t		xQueuePackageRx, xQueuePackageTx;
extern EventGroupHandle_t	communication_events;

#endif /* INCLUDES_COMMUNICATION_H_ */
