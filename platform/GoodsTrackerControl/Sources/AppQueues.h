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

#define		BIT_UPDATE_GPS	0x01
#define		BIT_UPDATE_AD	0x02
#define		BIT_UPDATE_ACCE	0x04

void initQueues(void);

extern const TickType_t /*xMainDelay,*/
						xCommunicationDelay,
						xDataDelay,
						xIHMDelay,
						xGPSDelay,
						xAccelDelay;


extern QueueHandle_t xQueueCom, xQueueDataTLM, xQueueLCD, xQueueAnswer;

extern TaskHandle_t xHandleMainTask, xHandleCommunicationTask,
		xHandleDataTask, xHandleIHMTask, xHandleGPSTask, xHandleAccelTask,xHandleCallBackTask,xHandleRunTxTask;


#endif /* SOURCES_APPQUEUES_H_ */