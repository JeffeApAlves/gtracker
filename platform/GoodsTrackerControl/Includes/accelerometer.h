/*
 * accelerometer.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_ACCELEROMETER_H_
#define SOURCES_ACCELEROMETER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct{

	int		x,y,z;
	float	x_g,y_g,z_g;

} Accelerometer;

void accelerometer_task(void);
void accelerometer_init(void);
void deInitAccelerometer(void);

#define clearAccelerometer(f) memset((void*)f,0,sizeof(Accelerometer));

extern QueueHandle_t	xQueueAcc;

extern TaskHandle_t xHandleAccelTask;

#endif /* SOURCES_ACCELEROMETER_H_ */
