/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_TELEMETRIA_H_
#define SOURCES_TELEMETRIA_H_


#include "Tank.h"
#include "GPS.h"
#include "Accelerometer.h"

#define		BIT_UPDATE_GPS				0x01
#define		BIT_UPDATE_TANK				0x02
#define		BIT_UPDATE_ACCELEROMETER	0x04
#define		BIT_AD_VALUE				0x08
//Eventos do clock
#define		BIT_UPDATE_LCD				0x10

typedef struct{

	GPS				GPS;
	Tank			Tank;
	Accelerometer	Accelerometer;

} Telemetria;

#define clearTelemetria(f) memset((void*)f,0,sizeof(Telemetria));
void tlm_notify_accelerometer(void);
void tlm_notify_tank(void);
void tlm_notify_gps(void);
void tlm_init(void);

extern	Telemetria			telemetria;
extern 	EventGroupHandle_t	tlm_events;

BaseType_t tank_notify_value(BaseType_t *xHigherPriorityTaskWoken);

#endif /* SOURCES_DATATLM_H_ */
