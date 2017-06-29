/*
 * accelerometer.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_ACCELEROMETER_H_
#define SOURCES_ACCELEROMETER_H_

enum {AXIS_X=0,AXIS_Y=1,AXIS_Z=2};

typedef struct {

	uint8_t	AcceXYZ[3];
	uint8_t	IncXYZ[3];

} DataAcce;

void read_accel(void);
void initAccel(void);

#define clearAcce(f) memset((void*)f,0,sizeof(DataAcce));

#endif /* SOURCES_ACCELEROMETER_H_ */
