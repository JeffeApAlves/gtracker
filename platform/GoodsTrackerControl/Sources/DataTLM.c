/*
 * DataTLM.c
 *
 *  Created on: 01/07/2017
 *      Author: Jefferson
 */

#include "Accelerometer.h"
#include "AppQueues.h"
#include "DataTLM.h"

DataTLM		dataTLM;


/**
 *
 * Atualiza a s informcoes de telemetria que estao na lista de menssagem xQueueDataTLM
 *
 */
void updateTLM(void){

	uint32_t ulNotifiedValue =0 ;

	if(xTaskNotifyWait( 0x0, BIT_UPDATE_GPS | BIT_UPDATE_ACCE | BIT_UPDATE_AD,  &ulNotifiedValue, TIMEOUT_TLM )==pdTRUE){

		if(ulNotifiedValue & BIT_UPDATE_GPS){

			updateDataGPS();
		}

		if(ulNotifiedValue & BIT_UPDATE_ACCE){

			updateDataAcce();
		}

		if(ulNotifiedValue & BIT_UPDATE_AD){

			updateDataLevel();
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
			dataTLM.Speed	= data->Speed;
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
