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

#include "CommunicationFrame.h"

#define	BIT_TX			0x01
#define	BIT_RX			0x02

void rxPackage_task();
void txPackage_task();

void putPackageRx(CommunicationPackage* package_rx);
void putPackageTx(CommunicationPackage* package_tx);

void communication_init(void);

// Handles das Queues
extern QueueHandle_t	xQueueRx, xQueueTx;

// Handles das tasks
extern TaskHandle_t 	xHandleRxTask,xHandleTxTask;

#endif /* INCLUDES_COMMUNICATION_H_ */