/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdlib.h>
#include <stdbool.h>

#include "XF1.h"
#include "AD1.h"
#include "MMA1.h"
#include "lock.h"
#include "gps.h"
#include "application.h"

static TaskHandle_t xTaskToNotify = NULL;

volatile	bool AD_finished=FALSE;
uint16_t	AD_Values[AD1_CHANNEL_COUNT];
Info		dataTLM;
int			_lock;
char		msg2send[SIZE_MAX_PAYLOAD];

void updateDataTLM(void){

	dataTLM.Lat			= nmeaFrame.Lat;
	dataTLM.Lng			= nmeaFrame.Lng;
	dataTLM.Acc[AXIS_X]	= 1;
	dataTLM.Acc[AXIS_Y]	= 2;
	dataTLM.Acc[AXIS_Z]	= 3;
	dataTLM.Inc[AXIS_X]	= 20;
	dataTLM.Inc[AXIS_Y]	= 21;
	dataTLM.Inc[AXIS_Z]	= 22;
	dataTLM.Level			= AD_Values[2];
	dataTLM.Speed			= 100;
	dataTLM.Lock			= _lock;
	strcpy(dataTLM.Date,	nmeaFrame.Date);
	strcpy(dataTLM.Time,	nmeaFrame.Time_UTC);

}
//-------------------------------------------------------------------------

ResultExec onLED(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send,"23/06/2017 19.52");

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAnalog(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send,"23/06/2017 19.52");

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onAccel(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send,"23/06/2017 19.52");

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onTouch(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send,"23/06/2017 19.52");

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onPWM(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send,"23/06/2017 19.52");

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//------------------------------------------------------------------------

ResultExec onTelemetry(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		Infor2String(&dataTLM,msg2send);

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

ResultExec onLock(DataFrame* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		//Colocar no minimo horario que foi executado o cmd
		strcpy(msg2send,"23/06/2017 19.52");

		decoderPayLoad(frame->payload);

		if(_lock){

			lock();

		}else{

			unLock();
		}

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

void read_Channels_AD(void){


//	AD_finished = FALSE;

	if(AD1_Measure(TRUE)==ERR_OK){

//		while (!AD_finished) {}

		if(AD1_GetValue16(AD_Values)==ERR_OK){

		}
	}
}
//------------------------------------------------------------------------

void read_accel(void) {

	MMA1_GetRaw8XYZ(dataTLM.Inc);
}
//------------------------------------------------------------------------

void initAccel(void){

	MMA1_Init();
	MMA1_Enable();
}
//------------------------------------------------------------------------

void Infor2String(Info* info,char* str_out){

	if(info!=NULL && str_out!= NULL){

		XF1_xsprintf(str_out,"%.8f:%.8f:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s:%s",
				info->Lat,
				info->Lng,
				info->Acc[AXIS_X],
				info->Acc[AXIS_Y],
				info->Acc[AXIS_Z],
				info->Inc[AXIS_X],
				info->Inc[AXIS_Y],
				info->Inc[AXIS_Z],
				info->Speed,
				info->Level,
				info->Lock,
				info->Time,
				info->Date);
	}
}
//------------------------------------------------------------------------

void decoderPayLoad(char* payload){

	List	list;

	str_split(&list, payload, CHAR_SEPARATOR);

	AsInteger(&_lock,		&list,0);

	removeList(&list);
}
//------------------------------------------------------------------------

void initInfo(Info* info){

	memset(info,0,sizeof(Info));
	memset(msg2send,0,sizeof(char)*SIZE_MAX_PAYLOAD);
}
//------------------------------------------------------------------------

void initCallBacks(void){

	setEventCMD(CMD_LED,		onLED);
	setEventCMD(CMD_ANALOG,		onAnalog);
	setEventCMD(CMD_ACC,		onAccel);
	setEventCMD(CMD_TOUCH,		onTouch);
	setEventCMD(CMD_PWM,		onPWM);
	setEventCMD(CMD_TELEMETRIA,	onTelemetry);
	setEventCMD(CMD_LOCK,		onLock);
}
//-------------------------------------------------------------------------
