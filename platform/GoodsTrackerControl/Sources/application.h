/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "protocol.h"
#include "Array.h"

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

ResultExec onAnalog(DataCom* cmd);
ResultExec onLED(DataCom* cmd);
ResultExec onPWM(DataCom* cmd);
ResultExec onTouch(DataCom* cmd);
ResultExec onAccel(DataCom* cmd);
ResultExec onTelemetry(DataCom* frame);
ResultExec onLock(DataCom* frame);

void updateDataTLM(void);
void read_Channels_AD(void);
void initCallBacks(void);
void Infor2String(Info* info,char* str_out);
void initInfo(void);
void decoderPayLoad(ArrayPayLoad* payload);

void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);

#define clearInfo(f) memset((void*)f,0,sizeof(Info));

#endif /* SOURCES_APPLICATION_H_ */
