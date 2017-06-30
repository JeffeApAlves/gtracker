/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "Data.h"
#include "protocol.h"
#include "Array.h"

typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;


ResultExec onAnalog(DataCom* cmd);
ResultExec onLED(DataCom* cmd);
ResultExec onPWM(DataCom* cmd);
ResultExec onTouch(DataCom* cmd);
ResultExec onAccel(DataCom* cmd);
ResultExec onTelemetry(DataCom* frame);
ResultExec onLock(DataCom* frame);

void App_Run(void);
void initCallBacks(void);
void Infor2String(Info* info,char* str_out);
void initInfo(void);
void decoderPayLoad(ArrayPayLoad* payload);

void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);


#endif /* SOURCES_APPLICATION_H_ */
