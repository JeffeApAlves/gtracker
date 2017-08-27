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
#include "DataTLM.h"
#include "NMEAFrame.h"
#include "Level.h"
#include "Accelerometer.h"
#include "application.h"

static 	int			_lock;
static ArrayPayLoad	msg2send;

ArrayPayLoad*	pAnswer = &msg2send;

void runApp(void){

	uint32_t ulNotifiedValue;

	//xTaskNotifyWait( 0x0, 0xFFFF ,  &ulNotifiedValue, portMAX_DELAY );

	xTaskNotifyWait( 0x0, BIT_RX_FRAME | BIT_UPDATE_GPS | BIT_UPDATE_ACCE | BIT_UPDATE_AD,  &ulNotifiedValue, portMAX_DELAY);

	execCMD(ulNotifiedValue);

	updateTLM(ulNotifiedValue);
}
//-------------------------------------------------------------------------

void execCMD(uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_RX_FRAME){

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

		//updateTLM();

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

void tlm2String(DataTLM* info,ArrayPayLoad* ans){

	if(info!=NULL && ans!= NULL){

		clearArrayPayLoad(ans);

		XF1_xsprintf(ans->Data,"%.8f:%.8f:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s:%s",
				info->Lat,
				info->Lng,
				info->Axis[AXIS_X],
				info->Axis[AXIS_Y],
				info->Axis[AXIS_Z],
				info->G[AXIS_X],
				info->G[AXIS_Y],
				info->G[AXIS_Z],
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

void answerTime(void){

	clearArrayPayLoad(&msg2send);

	AppendPayLoad(&msg2send,"23/06/2017 19.52");


	if(xQueueSendToBack( xQueueAnswer , ( void * ) &pAnswer, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleRunTxTask , BIT_TX , eSetBits );

	}else{

		//TODO erro na resposta
	}
}
//-------------------------------------------------------------------------

void answerTLM(void){

	tlm2String(&dataTLM,&msg2send);

	if(xQueueSendToBack( xQueueAnswer , ( void * ) &pAnswer, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleRunTxTask , BIT_TX , eSetBits );

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

void initApp(void){

	clearDataTLM(&dataTLM);
	clearArrayPayLoad(&msg2send);
}
//------------------------------------------------------------------------
