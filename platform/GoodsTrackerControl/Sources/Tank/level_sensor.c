/*
 * level_sensor.c
 *
 *  Created on: Oct 24, 2017
 *      Author: Jefferson
 */

#include "AD1.h"

#include "level_sensor.h"


static	uint16_t	ADValues[AD1_CHANNEL_COUNT];

volatile bool	AD_finished;

/**
 * Leitura do tanque
 *
 */
bool readValues(uint32_t* val){

	AD_finished = false;

	if(AD1_Measure(true)==ERR_OK){

		while (!AD_finished) {}

		if(AD1_GetValue16(&ADValues[0])==ERR_OK){

			*val = ADValues[0];
		}
	}
}
//------------------------------------------------------------------------

void level_sensor_init(void){

	AD_finished		= false;
}
//-----------------------------------------------------------------------------

