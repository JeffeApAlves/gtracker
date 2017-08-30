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
typedef ResultExec(*pCallBack)(DataCom*);

ResultExec onAnalog(DataCom* cmd);
ResultExec onLED(DataCom* cmd);
ResultExec onPWM(DataCom* cmd);
ResultExec onTouch(DataCom* cmd);
ResultExec onAccel(DataCom* cmd);
ResultExec onTelemetry(DataCom* frame);
ResultExec onLock(DataCom* frame);

void initApp(void);
void decoderLockPayLoad(ArrayPayLoad* payload);
void answerTime(void);
void answerTLM(void);
void execCMD(uint32_t ulNotifiedValue);
static void setHeaderInfo(DataCom* data);

void runMain(void);
void runApp(void);
pCallBack getCallBack(Resource* r);

#endif /* SOURCES_APPLICATION_H_ */
