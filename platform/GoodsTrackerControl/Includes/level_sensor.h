#ifndef LEVEL_SENSOR_H_
#define LEVEL_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>

#include "MCUC1.h"

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"

/* Drivers*/
#include "fsl_debug_console.h"
#include "fsl_smc.h"
#include "fsl_pmc.h"
#include "fsl_adc16.h"
#include "fsl_dmamux.h"
#include "fsl_lptmr.h"
#include "fsl_dma.h"


#define LED_Init()		LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()	LED_BLUE_TOGGLE()
#define LED_R_Init()	LED_RED_INIT(LOGIC_LED_OFF)
#define LED_R_On() 		LED_RED_ON()
#define LED_R_Off()		LED_RED_OFF()
#define LED_G_Init()	LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED_G_On()		LED_GREEN_ON()
#define LED_G_Off() 	LED_GREEN_OFF()


#define DEMO_ADC16_BASEADDR			ADC0
#define DEMO_ADC16_CHANNEL_GROUP	0U
#define DEMO_ADC16_CHANNEL			0U
#define ADC16_RESULT_REG_ADDR		(uint32_t)(&ADC0->R[0]) /* Get ADC16 result register address */

#define DEMO_DMA_BASEADDR			DMA0
#define DEMO_DMA_IRQ_ID				DMA0_IRQn
#define DEMO_DMA_IRQ_HANDLER_FUNC	DMA0_IRQHandler
#define DEMO_DMA_CHANNEL			0U
#define DEMO_DMA_ADC_SOURCE			kDmaRequestMux0ADC0
#define DEMO_DMAMUX_BASEADDR		DMAMUX0
#define DEMO_LPTMR_BASE				LPTMR0
#define DEMO_LPTMR_COMPARE_VALUE	200U				/* Low Power Timer interrupt time in miliseconds */
#define DEMO_ADC16_SAMPLE_COUNT		4U   				/* The ADC16 sample count */
#define AD1_CHANNEL_COUNT			1


#else
#include "AD1.h"
#include "LED_B.h"
#include "LED_G.h"
#include "LED_R.h"

#endif

void level_sensor_init(void);
bool readValues(uint32_t* valor);

#if MCUC1_CONFIG_NXP_SDK_2_0_USED==0

extern volatile bool	AD_finished;

#endif

#endif
