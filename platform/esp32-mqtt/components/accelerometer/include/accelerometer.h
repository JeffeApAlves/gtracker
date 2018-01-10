/*
 * accelerometer.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_ACCELEROMETER_H_
#define SOURCES_ACCELEROMETER_H_

#include <stdbool.h>

#include "rtos.h"

typedef struct{

	int		x,y,z;
	float	x_g,y_g,z_g;

} Accelerometer;

void accelerometer_random_data(void);
void accelerometer_deInit(void);
void accelerometer_init(void);
void accelerometer_task(void);
void accelerometer_sendResultQueue(void);
bool accelerometer_getdata(void);

#define clearAccelerometer(f) memset((void*)f,0,sizeof(Accelerometer));

// Handles das Queues
extern QueueHandle_t	xQueueAcc;

#endif /* SOURCES_ACCELEROMETER_H_ */
