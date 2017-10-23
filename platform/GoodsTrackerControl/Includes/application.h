/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "CommunicationFrame.h"
#include "Frame.h"
#include "Telemetria.h"
#include "protocol.h"

//bits de sinalização das notificações
typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;

/**
 * Ponteiro para as call backs
 */
typedef bool(*pCallBack)(CommunicationPackage*);
//typedef	TaskFunction_t	pCallBack;

void app_init(void);
void decoderLockPayLoad(PayLoad* payload);

#define HOOK_CMD(vFunction, parameters)	bool vFunction(CommunicationPackage* parameters)

pCallBack getCallBack(Resource r);

#endif /* SOURCES_APPLICATION_H_ */
