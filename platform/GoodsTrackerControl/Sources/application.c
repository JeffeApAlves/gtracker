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
#include "application.h"

volatile	bool AD_finished=FALSE;
uint16_t	AD_Values[AD1_CHANNEL_COUNT];
Info		dataInfo;
int			_lock;
char		msg2send[SIZE_MAX_PAYLOAD];

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

		dataInfo.Lat			= -23.591387;
		dataInfo.Lng			= -46.645126;
		dataInfo.Acc[AXIS_X]	= 1;
		dataInfo.Acc[AXIS_Y]	= 2;
		dataInfo.Acc[AXIS_Z]	= 3;
		dataInfo.Inc[AXIS_X]	= 4;
		dataInfo.Inc[AXIS_Y]	= 5;
		dataInfo.Inc[AXIS_Z]	= 6;
		dataInfo.Level			= 1000;
		dataInfo.Speed			= 100;
		dataInfo.Lock			= true;
		strcpy(dataInfo.Date,	"23/06/2017 19.52");

		Infor2String(&dataInfo,msg2send);

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

//	if(AD_finished){
//		if(AD1_GetValue16(AD_Values)==ERR_OK){
			//TODO
//		}
//	}

	if(AD1_Measure(FALSE)==ERR_OK){

		AD_finished = FALSE;

		while (!AD_finished) {}

		if(AD1_GetValue16(AD_Values)==ERR_OK){
			//TODO
		}
	}
}
//------------------------------------------------------------------------

void read_accel(void) {

	MMA1_GetRaw8XYZ(dataInfo.Inc);
}
//------------------------------------------------------------------------

void initAccel(void){

	MMA1_Init();
	MMA1_Enable();
}
//------------------------------------------------------------------------

void Infor2String(Info* info,char* str_out){

	if(str_out){

		XF1_xsprintf(str_out,"%.7f:%.7f:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s",
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
