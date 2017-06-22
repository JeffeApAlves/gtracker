/*
 * ihm.c
 *
 *  Created on: 20/06/2017
 *      Author: Fl�vio Soares
 */
#include <stdio.h>
#include "ihm.h"
#include "TSSin.h"
#include "lcd.h"

char *functionsArray[] = {
							"OPTION 1",
							"OPTION 3",
							"OPTION 2"
						 };

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
	UINT8 option;
	struct {
		ihmEvent event[IHM_MAX_EVENTS]; //TODO - USAR BUFFER CIRCULAR
		UINT8 head;
		UINT8 tail;
	} ihmEventBuffer;

} ihmStruct;

static ihmStruct ihmGlobal;

void ihm_initialize()
{
    TSSin_Configure(); /* initialize TSS library */

	LCDInit();
	LCDGotoXY(1, 2);
	LCDWriteString(".GOODSTRACKER.");
	LCDGotoXY(2, 1);
	LCDWriteString("VERSION 0.1");

	ihmGlobal.option = 0;

	/*Inicializa a estrutura de eventos*/
	unsigned char i;
	ihmGlobal.ihmEventBuffer.head = 0;
	ihmGlobal.ihmEventBuffer.tail = 0;
	for (i = 0; i < IHM_MAX_EVENTS; i++) {
		ihmGlobal.ihmEventBuffer.event[i].evType = IHM_EVENT_NONE;
		/*INICIALIZAMOS COMO J� TRATADO PARA N�O ENTRAR A 1a VEZ A TOA*/
		ihmGlobal.ihmEventBuffer.event[i].eventTreated = TRUE;
	}

}

void ihm_terminate()
{

}

static void ihm_process_events(ihmStruct *ihm)
{
	if (ihm) {
#if 0
		//TODO - ISSO � S� UMA VERS�O PRELIMINAR, O CERTO � TRATAR COMO RingBuffer
		UINT8 evmax = ihm->ihmEventBuffer.head;
		unsigned char i;
		for (i = 0; i < evmax; i++) {
			ihmEvent *ev = &ihm->ihmEventBuffer.event[i];
			ihm_process_event(ev);
		}
#endif
	}
}

static void ihm_process_event(ihmEvent *ev)
{
	if (ev) {

		switch(ev->evType) {
			case IHM_EVENT_NONE:
				break;
			case IHM_EVENT_CHOOSE_LEFT:
				LCDClear();
				LCDWriteString("ESQUERDA");
				break;
			case IHM_EVENT_CHOOSE_RIGHT:
				LCDClear();
				LCDWriteString("DIREITA");
				break;
			case IHM_EVENT_CHOOSE_OK:
				LCDClear();
				LCDWriteString("OK");
				break;
			default:
				break;
		}

	}
}

void ihm_loop()
{
  TSS_Task(); /* call TSS library to process touches */

  ihm_process_events(&ihmGlobal);
}

//TODO - IMPLEMENTAR CHAMADA PARA RECEBER EVENTOS EM GERAL, ABSTRAIR MELHOR O HARDWARE
//TODO - POR ENQUANTO FICA ASSIM PARA IMPLEMENTA��O B�SICA
int ihm_put_slide_event(TSS_CSASlider *event)
{
	  if (event == NULL)
		  return -1;

	  if (TSSin_cKey0.DynamicStatus.Movement)
	  {
		UINT8 evposit = ihmGlobal.ihmEventBuffer.head;
		if (evposit >= IHM_MAX_EVENTS)
			evposit = 0;
		else
			evposit++;

		ihmGlobal.ihmEventBuffer.head = evposit;

		ihmEvent *ev = &ihmGlobal.ihmEventBuffer.event[evposit];

		if (TSSin_cKey0.Events.Touch)
	    {
	      if (!(TSSin_cKey0.Events.InvalidPos))
	      {

	        LED_R_Put(0);
	        LED_G_Put(1);
	        LED_B_Put(0);

	        ev->evType = IHM_EVENT_CHOOSE_OK;
	      }
	    }

		if (TSSin_cKey0.DynamicStatus.Displacement > (UINT8)15)
		{

			if (TSSin_cKey0.DynamicStatus.Direction)
			{
		        LED_R_Put(1);
		        LED_G_Put(0);
		        LED_B_Put(0);

				ev->evType = IHM_EVENT_CHOOSE_LEFT;
			}
			else
			{
		        LED_R_Put(0);
		        LED_G_Put(0);
		        LED_B_Put(1);

		        ev->evType = IHM_EVENT_CHOOSE_RIGHT;
			}

		}

	  }

	return 0;
}