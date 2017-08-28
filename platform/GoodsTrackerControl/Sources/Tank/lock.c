/*
 * lock.c
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */


#include "LED_B.h"
#include "LED_G.h"
#include "LED_R.h"

void lock(void){

	LED_R_On();
	LED_G_Off();


}
//------------------------------------------------------------------------

void unLock(void){

	LED_G_On();
	LED_R_Off();
}
//------------------------------------------------------------------------



