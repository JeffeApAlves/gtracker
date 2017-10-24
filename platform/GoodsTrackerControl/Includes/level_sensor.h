/*
 * level_sensor.h
 *
 *  Created on: Oct 24, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_LEVEL_SENSOR_H_
#define INCLUDES_LEVEL_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>

#include "LED_B.h"
#include "LED_G.h"
#include "LED_R.h"

bool readValues(uint32_t* val);
void level_sensor_init(void);

extern volatile bool	AD_finished;

#endif /* INCLUDES_LEVEL_SENSOR_H_ */
