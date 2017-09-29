/*
 * led.h
 *
 *  Created on: 21 de set de 2017
 *      Author: jefferson
 */

#ifndef COMPONENTS_LED_BLINK_LED_H_
#define COMPONENTS_LED_BLINK_LED_H_

#include "esp_system.h"

#define BLINK_GPIO 2

void LED_init();
void blink_task(void *pvParameter);

#endif /* COMPONENTS_LED_BLINK_LED_H_ */
