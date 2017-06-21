/*
 * ihm.c
 *
 *  Created on: 20/06/2017
 *      Author: Flávio Soares
 */
#include "ihm.h"
#include "TSSin.h"
#include "lcd.h"

void initialize_ihm()
{
    TSSin_Configure(); /* initialize TSS library */

	LCDInit();
	LCDGotoXY(1, 6);
	LCDWriteString("TESTE RTOS");
	LCDGotoXY(2, 1);
	LCDWriteString("GOODSTRACKER");
}

void terminate_ihm()
{

}

void loop_ihm()
{

  TSS_Task(); /* call TSS library to process touches */
}
