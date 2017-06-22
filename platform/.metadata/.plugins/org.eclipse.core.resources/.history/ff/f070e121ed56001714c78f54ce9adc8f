/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "protocol.h"


typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;


ResultExec onAnalog(ParamCmd* cmd);
ResultExec onLED(ParamCmd* cmd);
ResultExec onPWM(ParamCmd* cmd);
ResultExec onTouch(ParamCmd* cmd);
ResultExec onAcel(ParamCmd* cmd);

void read_accel();
void read_Channels_AD();
void initCallBacks();
void initAccel();

extern volatile bool AD_finished;

#endif /* SOURCES_APPLICATION_H_ */
