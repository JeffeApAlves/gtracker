/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include <CommunicationFrame.h>
#include <Frame.h>
#include <Telemetria.h>
#include "protocol.h"

typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;

/**
 * Ponteiro para as call backs
 */
typedef ResultExec(*pCallBack)(CommunicationFrame*);

ResultExec onAnalog(CommunicationFrame* cmd);
ResultExec onLED(CommunicationFrame* cmd);
ResultExec onPWM(CommunicationFrame* cmd);
ResultExec onTouch(CommunicationFrame* cmd);
ResultExec onAccel(CommunicationFrame* cmd);
ResultExec onTelemetry(CommunicationFrame* frame);
ResultExec onLock(CommunicationFrame* frame);

void initApp(void);
void decoderLockPayLoad(PayLoad* payload);
void execCMD(uint32_t ulNotifiedValue);
static void setHeaderAnswer(CommunicationFrame* data);

void runMain(void);
void runApp(void);
pCallBack getCallBack(Resource r);

#endif /* SOURCES_APPLICATION_H_ */
