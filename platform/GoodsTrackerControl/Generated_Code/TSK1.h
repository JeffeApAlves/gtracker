/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : TSK1.h
**     CDE edition : Community
**     Project     : GoodsTrackerControl
**     Processor   : MKL25Z128VLK4
**     Component   : FreeRTOS_Tasks
**     Version     : Component 01.008, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-30, 14:53, # CodeGen: 52
**     Abstract    :
**
**     Settings    :
**          Component Name                                 : TSK1
**          FreeRTOS                                       : FRTOS1
**          Number of tasks                                : 7
**            FreeRTOS task                                : app_taskTask
**              Task name                                  : app_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**            FreeRTOS task                                : communication_taskTask
**              Task name                                  : communication_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**            FreeRTOS task                                : data_taskTask
**              Task name                                  : data_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**            FreeRTOS task                                : ihm_taskTask
**              Task name                                  : ihm_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**            FreeRTOS task                                : gps_taskTask
**              Task name                                  : gps_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**            FreeRTOS task                                : accel_taskTask
**              Task name                                  : accel_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**            FreeRTOS task                                : callback_taskTask
**              Task name                                  : callback_task
**              Stack size (plus configMINIMAL_STACK_SIZE) : 0
**              Initial Priority                           : 0
**     Contents    :
**         CreateTasks - void TSK1_CreateTasks(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Omar Isa� Pinales Ayala, 2014, all rights reserved.
**     http      : http://www.mcuoneclipse.com
**     This an open source software driver for Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file TSK1.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup TSK1_module TSK1 module documentation
**  @{
*/         

#ifndef __TSK1_H
#define __TSK1_H

/* MODULE TSK1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited components */
#include "FRTOS1.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
** ===================================================================
**     Method      :  TSK1_CreateTasks (component FreeRTOS_Tasks)
**     Description :
**         Creates the tasks.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TSK1_CreateTasks(void);


#ifdef __cplusplus
}  /* extern "C" */
#endif
/* END TSK1. */

#endif
/* ifndef __TSK1_H */
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
