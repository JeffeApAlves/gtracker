/*
 * i2c.h
 *
 *  Created on: Oct 25, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_I2C_H_
#define INCLUDES_I2C_H_

#include <stdint.h>
#include <stdbool.h>

#include "MCUC1.h"


#if MCUC1_CONFIG_NXP_SDK_2_0_USED

#include "MKL25Z4.h"
#include "Driver_I2C.h"

/* SDK Included Files */
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_i2c_cmsis.h"

#define I2C	Driver_I2C0
#define I2C_MASTER_BASE (I2C0_BASE)
#define I2C_MASTER_IRQN (I2C0_IRQn)
#define I2C_MASTER_CLK_SRC (I2C0_CLK_SRC)
#define I2C_MASTER_CLK_FREQ CLOCK_GetFreq((I2C0_CLK_SRC))
#define I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define I2C_NVIC_PRIO 1
#define I2C_MASTER_SLAVE_ADDR_7BIT (0x1DU)
#define I2C_BAUDRATE (100000) /* 100K */

typedef  void				i2c_handle;

#else

#include "PE_LDD.h"
#include "I2C2.h"

typedef  LDD_TDeviceData	i2c_handle;

#endif

#define I2C_RELEASE_SDA_PORT	PORTE
#define I2C_RELEASE_SCL_PORT	PORTE
#define I2C_RELEASE_SDA_GPIO	GPIOE
#define I2C_RELEASE_SDA_PIN		25U
#define I2C_RELEASE_SCL_GPIO	GPIOE
#define I2C_RELEASE_SCL_PIN		24U
#define I2C_RELEASE_BUS_COUNT	100U

typedef enum{

	MMA8451_ID		=0x1AU,
	FXOS8700_ID		=0xC7U

} id_device;

typedef struct {
	uint8_t			address;
	id_device		identification;
	volatile bool	dataReceivedFlg; 	/* set to TRUE by the interrupt if we have received data */
	volatile bool	dataTransmittedFlg; /* set to TRUE by the interrupt if we have set data */
	i2c_handle		*handle;				/* pointer to the device handle */
} i2c_state_t;

bool I2C_ReadBuffer(i2c_state_t* i2c_status,uint8_t addr, uint8_t *data, short dataSize);
bool I2C_Read(i2c_state_t* i2c_status,uint8_t addr, uint8_t *data);
bool I2C_Write(i2c_state_t* i2c_status,uint8_t addr, uint8_t val);
bool I2C_WhoAmI(i2c_state_t* i2c_status,uint8_t who_am_i_reg);
bool I2C_Init(i2c_state_t* i2c_status);
void I2C_Deinit(i2c_state_t* i2c_status);

void BOARD_i2c_ReleaseBus(void);

#endif /* INCLUDES_I2C_H_ */
