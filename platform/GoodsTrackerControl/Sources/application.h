/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "protocol.h"

enum {AXIS_X=0,AXIS_Y=1,AXIS_Z=2};

typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;

typedef struct{

		float	Lat;
		float	Lng;
		uint8_t	Acc[3];
		uint8_t	Inc[3];
		int		Speed ;
		int		Level;
		int		Lock;
		char	Time[11];
		char	Date[7];
	} Info;

ResultExec onAnalog(DataFrame* cmd);
ResultExec onLED(DataFrame* cmd);
ResultExec onPWM(DataFrame* cmd);
ResultExec onTouch(DataFrame* cmd);
ResultExec onAccel(DataFrame* cmd);
ResultExec onTelemetry(DataFrame* frame);
ResultExec onLock(DataFrame* frame);

void updateDataTLM(void);
void read_accel(void);
void read_Channels_AD(void);
void initCallBacks(void);
void initAccel(void);
void Infor2String(Info* info,char* str_out);
void initInfo(Info* info);
void decoderPayLoad(char* payload);

extern volatile bool AD_finished;

#endif /* SOURCES_APPLICATION_H_ */
