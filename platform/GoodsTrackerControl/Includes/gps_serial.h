/*
 * gps_serial.h
 *
 *  Created on: Oct 23, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_GPS_SERIAL_H_
#define INCLUDES_GPS_SERIAL_H_
#include <stdbool.h>

bool getGPSData(char* ch);
bool putGPSData(char data);
bool isAnyGPSData();
void gps_serial_init(void);


#endif /* INCLUDES_GPS_SERIAL_H_ */
