/*
 * led.c
 *
 *  Created on: 21 de set de 2017
 *      Author: jefferson
 */
#include <stdio.h>

#include "rtos.h"
#include "communication.h"
#include "led.h"

/**
 * Simples task que pisca o led a cada 1s
 *
 */

// Task Delay
static const TickType_t xTaskDelay	= (500 / portTICK_PERIOD_MS);

void LED_init(){

	xTaskCreate(&blink_task, "blink_task", 4096, NULL, 1, NULL);
}
//-------------------------------------------------------------------------------

void testCMD(void){

    PayLoad payload;

    clearPayLoad(&payload);
    AppendPayLoad(&payload,"");

    sendCMD(TRACKER_ADDRESS,CMD_TLM,&payload);
}
//-------------------------------------------------------------------------------

void blink_task(void *pvParameter) {

	gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(xTaskDelay);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(xTaskDelay);

        testCMD();
    }
}
//-------------------------------------------------------------------------------
