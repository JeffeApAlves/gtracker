/*
 * I2C.c
 *
 *  Created on: Oct 25, 2017
 *      Author: Jefferson
 */

#include "I2C.h"

static	volatile bool	completionFlag,nakFlag;

/* Endereçõs possiveis para o dispositivo FXOS8700 ou MMA8451 */
static const uint8_t ACCEL_ADDRESS[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

void BOARD_i2c_ReleaseBus(void);

/* Espera a transmissão */
static void I2C_wait_tx(i2c_state_t* i2c);

/* Espera a recepção */
static void I2C_wait_rx(i2c_state_t* i2c);

/* Espera a transferencia */
static bool I2C_wait_transfer(void);

/* Callback master event */
static void I2C_signal_event(uint32_t event);


inline bool I2C_Read(i2c_state_t* i2c_status,uint8_t addr, uint8_t *data) {

	return I2C_ReadBuffer(i2c_status,addr,data,1);
}
//--------------------------------------------------------------------------------------------

bool I2C_ReadBuffer(i2c_state_t* i2c_status,uint8_t addr, uint8_t *data, short dataSize) {

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

    if(I2C.MasterTransmit(i2c_status->address, &addr, 1U, false)==ARM_DRIVER_OK){

    	I2C_wait_transfer();

    	if(I2C.MasterReceive(i2c_status->address, data, dataSize, false)==ARM_DRIVER_OK){

    		return I2C_wait_transfer();

    	}else{

    		return false;
    	}
    }

#else

	/* Send I2C address plus register address to the I2C bus *without* a stop condition */

	if (I2C2_MasterSendBlock(i2c_status->handle, &addr, 1U, LDD_I2C_NO_SEND_STOP)==ERR_OK) {

		I2C_wait_tx(i2c_status);

		/* Receive InpData (1 byte) from the I2C bus and generates a stop condition to end transmission */
		if (I2C2_MasterReceiveBlock(i2c_status->handle, data, dataSize, LDD_I2C_SEND_STOP)==ERR_OK) {

			I2C_wait_rx(i2c_status);

			return true;
		}
	}

#endif

	return false;
}
//--------------------------------------------------------------------------------------------

bool I2C_Write(i2c_state_t* i2c_status,uint8_t addr, uint8_t val) {

	uint8_t writedata[2] = {addr, val};

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

    if(I2C.MasterTransmit(i2c_status->address, writedata, 2, false)==ARM_DRIVER_OK){

    	return I2C_wait_transfer();
    }
#else

	if (I2C2_MasterSendBlock(i2c_status->handle, &writedata, 2U, LDD_I2C_SEND_STOP)==ERR_OK) {

		I2C_wait_tx(i2c_status);

		return true;
	}

#endif

	return true;
}
//--------------------------------------------------------------------------------------------

static inline void I2C_wait_tx(i2c_state_t* i2c_status){

//	i2c_status->dataTransmittedFlg = false;

	while (!i2c_status->dataTransmittedFlg) {}  /* Wait until date is sent */

	i2c_status->dataTransmittedFlg = false;
}
//--------------------------------------------------------------------------------------------

static inline void I2C_wait_rx(i2c_state_t* i2c_status){

//	i2c_status->dataReceivedFlg = false;

	while (!i2c_status->dataReceivedFlg) {}  /* Wait until date is sent */

	i2c_status->dataReceivedFlg = false;
}
//--------------------------------------------------------------------------------------------


/*
 * Espera a transferencia ser completada
 *
 */
static inline bool I2C_wait_transfer(void){

	while ((!nakFlag) && (!completionFlag)){}

	nakFlag = false;

    if(completionFlag){

    	completionFlag= false;

    	return true;
    }

    return false;
}
//---------------------------------------------------------------------------------------

bool I2C_Init(i2c_state_t* i2c_status){

	bool initOK = false;

	memset(i2c_status,0 ,sizeof(i2c_state_t));

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

    BOARD_i2c_ReleaseBus();

    I2C.Initialize(I2C_signal_event);
    I2C.PowerControl(ARM_POWER_FULL);
    I2C.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST_PLUS);

//    NVIC_SetPriority(i2c_IRQN, I2C_NVIC_PRIO);
//    I2C0->C1 |= I2C_C1_IICIE_MASK; /* enable device specific interrupt flag */
//    NVIC_EnableIRQ(i2c_IRQN);

    //TODO - verficar inicialiacao

    initOK = true;

#else

	i2c_status->handle = I2C2_Init(i2c_status);
	initOK =  i2c_status->handle!=NULL;

#endif

	return initOK;
}
//--------------------------------------------------------------------------------------------

void I2C_Deinit(i2c_state_t* i2c_status){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	I2C.Uninitialize();
#else
	I2C2_Deinit(i2c_status->handle);
#endif
}
//--------------------------------------------------------------------------------------------

/*
 * Procura dispositivos da lista ACCEL_ADDRESS no barramento I2C
 *
 */
static void search_device(i2c_state_t* i2c_status,uint8_t who_am_i_reg){
#if MCUC1_CONFIG_NXP_SDK_2_0_USED

	uint8_t addr_found		= 0;
    uint8_t i				= 0;
    uint8_t array_size		= sizeof(ACCEL_ADDRESS) / sizeof(ACCEL_ADDRESS[0]);

    for (i = 0; i < array_size; i++){

    	completionFlag	= false;
    	nakFlag			= false;

        I2C.MasterTransmit(ACCEL_ADDRESS[i], &who_am_i_reg, 1, true);

        if (I2C_wait_transfer()){

            addr_found = ACCEL_ADDRESS[i];
            break;
        }
    }

    i2c_status->address =  addr_found;
#else
    //TODO valor fixo do endereço
    i2c_status->address = ACCEL_ADDRESS[1];
#endif
}
//----------------------------------------------------------------------------------------

/**
 * Identifica o dispositivo
 *
 */
bool I2C_WhoAmI(i2c_state_t* i2c_status,uint8_t who_am_i_reg){

	bool read_ok = false;

	search_device(i2c_status,who_am_i_reg);

    if (i2c_status->address){

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

    	completionFlag	= false;
    	nakFlag			= false;

    	I2C.MasterReceive(i2c_status->address, &i2c_status->identification, 1, false);

    	read_ok = I2C_wait_transfer();


#else
    	read_ok =  I2C_Read(i2c_status,who_am_i_reg,&i2c_status->identification);
#endif

    	if (read_ok){

    		if (i2c_status->identification == FXOS8700_ID){

                return true;

            } else if (i2c_status->identification == MMA8451_ID){

                return true;

            } else {

                return false;
            }
        } else {

            return false;
        }

    } else {

        return false;
    }
}
//---------------------------------------------------------------------------------------

#if MCUC1_CONFIG_NXP_SDK_2_0_USED

/*
 * Delay a bit
 *
 */
static inline void i2c_release_bus_delay(void){

    uint32_t i = 0;

    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++){
        __NOP();
    }
}
//-------------------------------------------------------------------------------------------

/**
 * Hook de eventos
 *
 */
static void I2C_signal_event(uint32_t event){

    if (event == ARM_I2C_EVENT_TRANSFER_DONE){

        completionFlag = true;
    }

    if (event == ARM_I2C_EVENT_ADDRESS_NACK){

        nakFlag = true;
    }
}
//---------------------------------------------------------------------------------------

/*
 * Le frequencia
 *
 */
uint32_t i2c0_GetFreq(void){

    return CLOCK_GetFreq(I2C0_CLK_SRC);
}
//---------------------------------------------------------------------------------------

/**
 *
 * Inicializa barramento
 *
 */
void BOARD_i2c_ReleaseBus(void){

    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SDA_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (i = 0; i < 9; i++){

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}
//---------------------------------------------------------------------------------------

#endif
