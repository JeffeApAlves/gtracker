/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "DataTLM.h"
#include "protocol.h"
#include "Array.h"

typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;

/**
 * Ponteiro para as call backs
 */
typedef ResultExec(*pCallBack)(ArrayPayLoad*);


ResultExec onAnalog(ArrayPayLoad* cmd);
ResultExec onLED(ArrayPayLoad* cmd);
ResultExec onPWM(ArrayPayLoad* cmd);
ResultExec onTouch(ArrayPayLoad* cmd);
ResultExec onAccel(ArrayPayLoad* cmd);
ResultExec onTelemetry(ArrayPayLoad* frame);
ResultExec onLock(ArrayPayLoad* frame);

void runApp(void);
void Infor2String(DataTLM* info,ArrayPayLoad* ans);
void initApp(void);
void decoderLockPayLoad(ArrayPayLoad* payload);
void answerTime(void);
void answerTLM(void);

void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);
void runCallBack(void);
pCallBack getCallBack(Resource* r);

#endif /* SOURCES_APPLICATION_H_ */
