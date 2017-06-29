/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdlib.h>
#include <stdbool.h>

#include "AppQueues.h"
#include "XF1.h"
#include "lock.h"
#include "NMEAFrame.h"
#include "Level.h"
#include "Accelerometer.h"
#include "application.h"

//static TaskHandle_t xTaskToNotify = NULL;

Info		dataTLM;
int			_lock;
char		msg2send[SIZE_MAX_PAYLOAD];

void updateDataTLM(void){

	if(xQueueGPS!=0){

		NMEAFrame* info;

		if( xQueueReceive( xQueueGPS, &(info), ( TickType_t ) 1 ) ){

			dataTLM.Lat				= info->Lat;
			dataTLM.Lng				= info->Lng;
			strcpy(dataTLM.Date,	info->Date);
			strcpy(dataTLM.Time,	info->Time_UTC);
		}
	}

	if(xQueueAcce!=0){

		AcceInfo* info;

		if( xQueueReceive( xQueueAcce, &(info), ( TickType_t ) 1 ) ){

			dataTLM.Acc[AXIS_X]		= info->AcceXYZ[AXIS_X];
			dataTLM.Acc[AXIS_Y]		= info->AcceXYZ[AXIS_Y];
			dataTLM.Acc[AXIS_Z]		= info->AcceXYZ[AXIS_Z];
			dataTLM.Inc[AXIS_X]		= info->IncXYZ[AXIS_X];
			dataTLM.Inc[AXIS_Y]		= info->IncXYZ[AXIS_Y];
			dataTLM.Inc[AXIS_Z]		= info->IncXYZ[AXIS_Z];
		}
	}

	if(xQueueAD!=0){

		ADInfo* info;

		if( xQueueReceive( xQueueAD, &(info), ( TickType_t ) 1 ) ){

			dataTLM.Level			= info->Level;
		}
	}

	dataTLM.Speed			= 100;
	dataTLM.Lock			= _lock;
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

		if(_lock){

			lock();

		}else{

			unLock();
		}

		//Colocar no minimo horario que foi executado o cmd
		strcpy(msg2send,"23/06/2017 19.52");

		decoderPayLoad(frame->payload);

		doAnswer(msg2send);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
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

	memset((void*)info,0,sizeof(Info));
	memset((void*)msg2send,0,sizeof(char)*SIZE_MAX_PAYLOAD);
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
