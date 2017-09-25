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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "TSS_API.h"

typedef enum {
	IHM_EVENT_NONE,
	IHM_EVENT_CHOOSE_LEFT,
	IHM_EVENT_CHOOSE_RIGHT,
	IHM_EVENT_CHOOSE_OK,
} ihmEventType;

typedef struct {
	bool eventTreated;
	ihmEventType evType;
} ihmEvent;

#define IHM_MAX_EVENTS 16
typedef struct {
	uint8_t option;
	struct {
		ihmEvent event[IHM_MAX_EVENTS]; //TODO - USAR BUFFER CIRCULAR
		uint8_t head;
		uint8_t tail;
	} ihmEventBuffer;

} ihmStruct;


/**
 * CHAMANDO ESTA ESTRUTURA DE IHM (INTERFACE HOMEM-M�QUINA) EM VEZ DE
 * MMI (MEN-MACHINE INTERFACE) POR PURO GOSTO PESSOAL... :)
 */

/**
 *
 */
void initIHM();

/**
 *
 */
void deInitIHM();

/**
 *
 */
void ihm_task();

/**
 *
 */
int ihm_put_slide_event(TSS_CSASlider *event);

void ihm_process_events(ihmStruct *ihm);

void initEvents();

void printLCD(int linha,int coluna,char* str);
void printClock(void);
void showSplah();
void printXYZ();

//extern QueueHandle_t	xQueueLCD;

extern TaskHandle_t		xHandleIHMTask;


#endif /* SOURCES_IHM_H_ */
