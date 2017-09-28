/*
 * consumer.h
 *
 *  Created on: Sep 24, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_CONSUMER_H_
#define INCLUDES_CONSUMER_H_

#include <stdint.h>

#include "Telemetria.h"

void updateTLM(Telemetria* tlm,uint32_t ulNotifiedValue);

#endif /* INCLUDES_CONSUMER_H_ */
