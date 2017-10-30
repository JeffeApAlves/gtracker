/*
 * ihm.h
 *
 *  Created on: 20/06/2017
 *      Author: Fl�vio Soares
 */
#ifndef SOURCES_IHM_H_
#define SOURCES_IHM_H_

#include <stdint.h>
#include <stdbool.h>

#include "MCUC1.h"
#include "XF1.h"

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
#include "fsl_port.h"
#include "fsl_gpio.h"

#define KEY_GPIO			GPIOD
#define KEY_PORT 			PORTD
#define SW_IRQ 				PORTD_IRQn
#define SW_IRQ_HANDLER 		PORTD_IRQHandler
#define SW_NAME "SW"
#endif

#define KEY_GPIO_PIN		7U
#define KEY_GPIO_MASK		(1 << KEY_GPIO_PIN)
#define KEY_INPUT			(~KEY)

// Tempo splah inicial
#define	TIME_SPLASH			1

//Eventos do clock
#define	BIT_UPDATE_LCD_CLOCK	0x01
#define	BIT_UPDATE_LCD_STAT_COM	0x02
#define	BIT_UPDATE_LCD_STAT_GPS	0x04
#define	BIT_UPDATE_LCD_TANK		0x10
#define	BIT_UPDATE_LCD_GPS		0x20
#define	BIT_UPDATE_LCD_XYZ		0x40
typedef enum{

	SCREEN_SPLASH,
	SCREEN_CLOCK,
	SCREEN_ACCE,
	SCREEN_TANK,
	SCREEN_GPS,
	SCREEN_STAT_COM,
	SCREEN_STAT_GPS,

	NUM_OF_SCREEN,

}screen;

/**
 * Inicializa interface homem máquina
 */
void ihm_init(void);

/**
 *
 */
void ihm_deInit(void);

/**
 * Task paara gerenciamento do ihm
 */
void printLCD(int linha,int coluna,char* str);
void printClock(void);
void printSplash(void);
void printAccelerometer(void);
void printStatCom(void);
void printStatGPS(void);
void printTank(void);
void printGPS(void);

void ihm_task(void);
void ihm_handle_update(void);
void ihm_notify_screen_stat(void);
void ihm_notify_screen_tlm(void);
void ihm_set_active_screen(screen s);
void readKey(void);

#define	TO_STRING	XF1_xsprintf
//#define	TO_STRING	sprintf

#endif /* SOURCES_IHM_H_ */
