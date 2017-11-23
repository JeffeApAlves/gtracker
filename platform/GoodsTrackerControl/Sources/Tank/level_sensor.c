/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>
#include <stdlib.h>

#include "level_sensor.h"

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

/* DMA */
dma_handle_t g_DMA_Handle;              	// DMA handler
dma_transfer_config_t g_transferConfig;		// DMA transfer config

/* Sample */
static uint32_t	g_adc16SampleDataArray[DEMO_ADC16_SAMPLE_COUNT];// ADC value array


#else
static	uint16_t	ADValues[AD1_CHANNEL_COUNT];

#endif


void BOARD_ConfigTriggerSource(void);

/*!
 * @brief Inicializa o LPTimer.
 *
 */
static void LPTMR_Configuration(void);

/*!
 * @brief Incicializa o ADCx para HW trigger.
 *
 */
static void ADC16_Configuration(void);

/*!
 * @brief Inicializa o DMA para modo  async.
 *
 */
static void DMA_Configuration(void);

/*!
 * @brief Calculo média amostras.
 *
 */
static uint32_t calcAverage(void);


/**
 * Leitura do tanque
 *
 */
bool readValues(uint32_t* val){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	LED_TOGGLE();

	*val = calcAverage();

	return true;

#else

	if(AD1_GetValue16(&ADValues[0])==ERR_OK){

		*val = ADValues[0];

		return true;
	}

	return false;

#endif
}
//------------------------------------------------------------------------

bool start_measure(void){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
	return true;
#endif

	return	AD1_Measure(true)==ERR_OK;
}
//------------------------------------------------------------------------

void level_sensor_init(void){

	LED_G_Init();
	LED_R_Init();

#if MCUC1_CONFIG_NXP_SDK_2_0_USED
    LED_Init();

    /* Set to allow entering vlps mode */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);

    /* Inicializa ADC */
    ADC16_Configuration();

    /* Inicializa DMA */
    DMA_Configuration();

    /* Inicializa o HW trigger source */
    LPTMR_Configuration();

    /* Inicializa SIM para ADC hw trigger source selection */
    BOARD_ConfigTriggerSource();

#endif

}
//-----------------------------------------------------------------------------

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

static uint32_t calcAverage(void){

	uint32_t	i = 0U;
    uint32_t	average = 0U;

    // Calcula  a media baseado nas amostras
    for (i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++){

    	average += g_adc16SampleDataArray[i];
        g_adc16SampleDataArray[i] = 0U;
    }

    return (average / DEMO_ADC16_SAMPLE_COUNT);
}
//-----------------------------------------------------------------------------

void BOARD_ConfigTriggerSource(void){

    /* Configure SIM for ADC hw trigger source selection */
    SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(14) | SIM_SOPT7_ADC0ALTTRGEN(1);
}
//------------------------------------------------------------------------

/* Enable the trigger source of LPTimer */
static void LPTMR_Configuration(void){

    lptmr_config_t lptmrUserConfig;

    LPTMR_GetDefaultConfig(&lptmrUserConfig);
    /* Init LPTimer driver */
    LPTMR_Init(DEMO_LPTMR_BASE, &lptmrUserConfig);

    /* Set the LPTimer period */
    LPTMR_SetTimerPeriod(DEMO_LPTMR_BASE, DEMO_LPTMR_COMPARE_VALUE);
}
//-----------------------------------------------------------------------------

static void ADC16_Configuration(void){

    adc16_config_t adcUserConfig;
    adc16_channel_config_t adcChnConfig;
    /*
    * Initialization ADC for
    * 16bit resolution, interrupt mode, hw trigger enabled.
    * normal convert speed, VREFH/L as reference,
    * disable continuous convert mode.
    */
    ADC16_GetDefaultConfig(&adcUserConfig);
    adcUserConfig.resolution = kADC16_Resolution16Bit;
    adcUserConfig.enableContinuousConversion = false;
    adcUserConfig.clockSource = kADC16_ClockSourceAsynchronousClock;

    adcUserConfig.longSampleMode = kADC16_LongSampleCycle24;
    adcUserConfig.enableLowPower = true;
#if ((defined BOARD_ADC_USE_ALT_VREF) && BOARD_ADC_USE_ALT_VREF)
    adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
    ADC16_Init(DEMO_ADC16_BASEADDR, &adcUserConfig);

#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    /* Auto calibration */
    ADC16_DoAutoCalibration(DEMO_ADC16_BASEADDR);
#endif

    adcChnConfig.channelNumber = DEMO_ADC16_CHANNEL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = false;
#endif
    adcChnConfig.enableInterruptOnConversionCompleted = false;
    /* Configure channel 0 */
    ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &adcChnConfig);
    /* Enable hardware trigger  */
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASEADDR, true);
    /* Enable DMA */
    ADC16_EnableDMA(DEMO_ADC16_BASEADDR, true);
}
//-----------------------------------------------------------------------------

static void DMA_Configuration(void){

    /* Configure DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_ADC_SOURCE); /* Map ADC source to channel 0 */
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);

    DMA_Init(DEMO_DMA_BASEADDR);
    DMA_CreateHandle(&g_DMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    DMA_PrepareTransfer(&g_transferConfig, (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
                        (void *)g_adc16SampleDataArray, sizeof(uint32_t), sizeof(g_adc16SampleDataArray),
                        kDMA_PeripheralToMemory);
    /* Setup transfer */
    DMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, &g_transferConfig);
    /* Enable interrupt when transfer is done. */
    DMA_EnableInterrupts(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    /* Enable async DMA request. */
    DMA_EnableAsyncRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, true);
    /* Forces a single read/write transfer per request. */
    DMA_EnableCycleSteal(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, true);
    /* Enable transfer. */
    DMA_StartTransfer(&g_DMA_Handle);
    /* Enable IRQ. */
    NVIC_EnableIRQ(DEMO_DMA_IRQ_ID);
}
//-----------------------------------------------------------------------------

void DEMO_DMA_IRQ_HANDLER_FUNC(void){

    /* Stop trigger */
    LPTMR_StopTimer(DEMO_LPTMR_BASE);
    /* Clear transaction done interrupt flag */
    DMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kDMA_TransactionsDoneFlag);
    /* Setup transfer */
    DMA_PrepareTransfer(&g_transferConfig, (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
                        (void *)g_adc16SampleDataArray, sizeof(uint32_t), sizeof(g_adc16SampleDataArray),
                        kDMA_PeripheralToMemory);
    DMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, &g_transferConfig);
}
//-----------------------------------------------------------------------------
#endif
