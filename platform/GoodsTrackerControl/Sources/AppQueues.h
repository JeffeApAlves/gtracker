/*
 * AppQueues.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPQUEUES_H_
#define SOURCES_APPQUEUES_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


void initQueues(void);

extern QueueHandle_t xQueueGPS, xQueueCom, xQueueAcce, xQueueLCD, xQueuePayload,
		xQueueAD;

extern TaskHandle_t xHandleMainTask, xHandleCommunicationTask,
		xHandleDataTask, xHandleIHMTask, xHandleGPSTask, xHandleAccelTask;


#endif /* SOURCES_APPQUEUES_H_ */
