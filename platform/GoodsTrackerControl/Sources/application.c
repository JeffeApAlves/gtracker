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

Info			dataTLM;
int				_lock;
ArrayPayLoad	msg2send;

void updateDataTLM(void){

	updateDataGPS();

	updateDataAcce();

	updateDataLevel();

	dataTLM.Speed	= 100;
	dataTLM.Lock	= _lock;
}
//-------------------------------------------------------------------------

void updateDataGPS(void) {

	if (xQueueGPS != 0) {

		DataNMEA* data;

		if (xQueueReceive(xQueueGPS, &(data), (TickType_t ) 1)) {

			dataTLM.Lat = data->Lat;
			dataTLM.Lng = data->Lng;
			strcpy(dataTLM.Date, data->Date);
			strcpy(dataTLM.Time, data->Time_UTC);
		}
	}
}
//-------------------------------------------------------------------------

void updateDataAcce(void) {

	if (xQueueAcce != 0) {

		DataAcce* data;

		if (xQueueReceive(xQueueAcce, &(data), (TickType_t ) 1)) {

			dataTLM.Acc[AXIS_X] = data->AcceXYZ[AXIS_X];
			dataTLM.Acc[AXIS_Y] = data->AcceXYZ[AXIS_Y];
			dataTLM.Acc[AXIS_Z] = data->AcceXYZ[AXIS_Z];
			dataTLM.Inc[AXIS_X] = data->IncXYZ[AXIS_X];
			dataTLM.Inc[AXIS_Y] = data->IncXYZ[AXIS_Y];
			dataTLM.Inc[AXIS_Z] = data->IncXYZ[AXIS_Z];
		}
	}
}
//-------------------------------------------------------------------------

void updateDataLevel(void) {

	if (xQueueAD != 0) {

		DataAD* data;

		if (xQueueReceive(xQueueAD, &(data), (TickType_t ) 1)) {

			dataTLM.Level = data->Level;
		}
	}
}
//-------------------------------------------------------------------------

ResultExec onLED(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send.Data,"23/06/2017 19.52");

		doAnswer(msg2send.Data);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAnalog(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send.Data,"23/06/2017 19.52");

		doAnswer(msg2send.Data);

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onAccel(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send.Data,"23/06/2017 19.52");

		doAnswer(msg2send.Data);

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onTouch(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send.Data,"23/06/2017 19.52");

		doAnswer(msg2send.Data);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onPWM(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		strcpy(msg2send.Data,"23/06/2017 19.52");

		doAnswer(msg2send.Data);

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//------------------------------------------------------------------------

ResultExec onTelemetry(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		Infor2String(&dataTLM,msg2send.Data);

		doAnswer(msg2send.Data);

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

ResultExec onLock(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		if(_lock){

			lock();

		}else{

			unLock();
		}

		//Colocar no minimo horario que foi executado o cmd
		strcpy(msg2send.Data,"23/06/2017 19.52");

		decoderPayLoad(&frame->PayLoad);

		doAnswer(msg2send.Data);

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

void decoderPayLoad(ArrayPayLoad* payload){

	List	list;

	str_split(&list, payload->Data, CHAR_SEPARATOR);

	AsInteger(&_lock,		&list,0);

	removeList(&list);
}
//------------------------------------------------------------------------

void initInfo(void){

	clearInfo(&dataTLM);
	clearArrayPayLoad(&msg2send);
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
