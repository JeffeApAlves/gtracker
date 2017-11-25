/* ###################################################################
**     Filename    : main.c
**     Project     : GoodsTrackerControl
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-09, 13:34, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
** Ferramentas de debug
**			https://www.nxp.com/docs/en/quick-reference-guide/MCUXpresso_IDE_FreeRTOS_Debug_Guide.pdf
**			https://percepio.com/docs/FreeRTOS/manual/Recorder.html#Trace_Recorder_Library_Snapshot_Mode
**			https://mcuoneclipse.com/2017/03/18/better-freertos-debugging-in-eclipse/
**			https://mcuoneclipse.com/2016/04/09/freertos-thread-debugging-with-eclipse-and-openocd/
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */

///* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "FRTOS1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "LED_G.h"
#include "LEDpin2.h"
#include "BitIoLdd2.h"
#include "LED_R.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "LED_B.h"
#include "LEDpin3.h"
#include "BitIoLdd3.h"
#include "LCDout.h"
#include "EN1.h"
#include "BitIoLdd6.h"
#include "RS1.h"
#include "BitIoLdd7.h"
#include "DB41.h"
#include "BitIoLdd12.h"
#include "DB51.h"
#include "BitIoLdd13.h"
#include "DB61.h"
#include "BitIoLdd14.h"
#include "DB71.h"
#include "BitIoLdd15.h"
#include "XF1.h"
#include "AS2.h"
#include "ASerialLdd2.h"
#include "WAIT1.h"
#include "MCUC1.h"
#include "UTIL1.h"
#include "WAIT2.h"
#include "WAIT3.h"
#include "WAIT4.h"
#include "I2C2.h"
#include "AD1.h"
#include "AdcLdd1.h"
#include "RTC1.h"
#include "EInt1.h"
#include "ExtIntLdd1.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "Telemetria.h"
#include "application.h"
#include "ihm.h"
#include "clock.h"
#include "communication.h"
#include "accelerometer.h"
#include "gps.h"

void sys_init(void){

	tlm_init();

	ihm_init();

	tank_init();

	gps_init();

	accelerometer_init();

	clock_init();

	communication_init();

	app_init();
}
//-----------------------------------------------------------------------------------------------

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/

	PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

	sys_init();

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/
//----------------------------------------------------------------------------------

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
