/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_TELEMETRIA_H_
#define SOURCES_TELEMETRIA_H_

#include <string.h>

#include "GPS.h"
#include "Level.h"
#include "Accelerometer.h"

typedef struct{

	DataGPS			GPS;
	Tank			Tank;
	Accelerometer	Accelerometer;

} Telemetria;

void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);
void updateTLM(uint32_t ulNotifiedValue);

#define clearTelemetria(f) memset((void*)f,0,sizeof(Telemetria));

extern Telemetria	telemetria;

#endif /* SOURCES_DATATLM_H_ */
