/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include <Telemetria.h>
#include "protocol.h"
#include "Array.h"
#include "DataFrame.h"

typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;

/**
 * Ponteiro para as call backs
 */
typedef ResultExec(*pCallBack)(DataFrame*);

ResultExec onAnalog(DataFrame* cmd);
ResultExec onLED(DataFrame* cmd);
ResultExec onPWM(DataFrame* cmd);
ResultExec onTouch(DataFrame* cmd);
ResultExec onAccel(DataFrame* cmd);
ResultExec onTelemetry(DataFrame* frame);
ResultExec onLock(DataFrame* frame);

void initApp(void);
void decoderLockPayLoad(PayLoad* payload);
void execCMD(uint32_t ulNotifiedValue);
static void setHeaderAnswer(DataFrame* data);

void runMain(void);
void runApp(void);
pCallBack getCallBack(Resource r);

#endif /* SOURCES_APPLICATION_H_ */
