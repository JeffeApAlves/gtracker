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

DataTLM			dataTLM;
int				_lock;
ArrayPayLoad	msg2send;
ArrayPayLoad*	pAnswer = &msg2send;

void runApp(void){

	uint32_t ulNotifiedValue;

	xTaskNotifyWait( 0x0, UPDATE_GPS | UPDATE_ACCE | UPDATE_AD,  &ulNotifiedValue, portMAX_DELAY );


	if(ulNotifiedValue & UPDATE_GPS){

		updateDataGPS();
	}

	if(ulNotifiedValue & UPDATE_ACCE){

		updateDataAcce();
	}

	if(ulNotifiedValue & UPDATE_AD){

		updateDataLevel();
	}

	dataTLM.Speed	= 100;
	dataTLM.Lock	= _lock;
}
//-------------------------------------------------------------------------

void runCallBack(void){

	uint32_t ulNotifiedValue;

	xTaskNotifyWait( 0x0, 0xFFFF ,  &ulNotifiedValue, portMAX_DELAY );

	DataCom* data;

	if (xQueueReceive(xQueueCom, &(data), (TickType_t ) 1)) {

		if(data!=NULL) {

			pCallBack cb = getCallBack(&data->resource);

			if(cb!=NULL){

				if(cb(&data->PayLoad) == CMD_RESULT_EXEC_SUCCESS) {

				}
				else {

				}
			}
		}
	}
}
//-------------------------------------------------------------------------

void updateDataGPS(void) {

	if (xQueueDataTLM != 0) {

		DataTLM* data;

		if (xQueueReceive(xQueueDataTLM, &(data), (TickType_t ) 1)) {

			dataTLM.Lat = data->Lat;
			dataTLM.Lng = data->Lng;
			strcpy(dataTLM.Date, data->Date);
			strcpy(dataTLM.Time, data->Time);
		}
	}
}
//-------------------------------------------------------------------------

void updateDataAcce(void) {

	if (xQueueDataTLM != 0) {

		DataTLM* data;

		if (xQueueReceive(xQueueDataTLM, &(data), (TickType_t ) 1)) {

			dataTLM.Acc[AXIS_X] = data->Acc[AXIS_X];
			dataTLM.Acc[AXIS_Y] = data->Acc[AXIS_Y];
			dataTLM.Acc[AXIS_Z] = data->Acc[AXIS_Z];
			dataTLM.Inc[AXIS_X] = data->Inc[AXIS_X];
			dataTLM.Inc[AXIS_Y] = data->Inc[AXIS_Y];
			dataTLM.Inc[AXIS_Z] = data->Inc[AXIS_Z];
		}
	}
}
//-------------------------------------------------------------------------

void updateDataLevel(void) {

	if (xQueueDataTLM != 0) {

		DataTLM* data;

		if (xQueueReceive(xQueueDataTLM, &(data), (TickType_t ) 1)) {

			dataTLM.Level = data->Level;
		}
	}
}
//-------------------------------------------------------------------------

ResultExec onLED(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAnalog(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onAccel(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onTouch(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onPWM(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//------------------------------------------------------------------------

ResultExec onTelemetry(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTLM();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

ResultExec onLock(ArrayPayLoad* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		decoderLockPayLoad(frame);

		if(_lock){

			lock();

		}else{

			unLock();
		}

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

void Infor2String(DataTLM* info,ArrayPayLoad* ans){

	if(info!=NULL && ans!= NULL){

		XF1_xsprintf(ans->Data,"%.8f:%.8f:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s:%s",
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

		ans->Count = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

void decoderLockPayLoad(ArrayPayLoad* payload){

	List	list;

	str_split(&list, payload->Data, CHAR_SEPARATOR);

	AsInteger(&_lock,		&list,0);

	removeList(&list);
}
//------------------------------------------------------------------------

void initApp(void){

	clearDataTLM(&dataTLM);
	clearArrayPayLoad(&msg2send);
}
//------------------------------------------------------------------------

void answerTime(void){

	clearArrayPayLoad(&msg2send);

	AppendPayLoad(&msg2send,"23/06/2017 19.52");

	if(xQueueSendToBack( xQueuePayload , ( void * ) &pAnswer, ( TickType_t ) 1 ) ){

	}else{

		//TODO erro na resposta
	}
}
//-------------------------------------------------------------------------

void answerTLM(void){

	clearArrayPayLoad(&msg2send);

	Infor2String(&dataTLM,&msg2send);

	if(xQueueSendToBack( xQueuePayload , ( void * ) &pAnswer, ( TickType_t ) 1 ) ){

	}else{

		//TODO erro na resposta
	}
}
//-------------------------------------------------------------------------

pCallBack getCallBack(Resource* r) {

	pCallBack	cb = NULL;

	switch(r->id){

		case CMD_LED:		cb = onLED;			break;
		case CMD_ANALOG:	cb = onAnalog;		break;
		case CMD_PWM:		cb = onPWM;			break;
		case CMD_ACC:		cb = onAccel;		break;
		case CMD_TOUCH:		cb = onTouch;		break;
		case CMD_TLM:		cb = onTelemetry;	break;
		case CMD_LOCK:		cb = onLock;		break;
		case CMD_LCD:		cb = NULL;			break;
	}

	return cb;
}
//------------------------------------------------------------------------
