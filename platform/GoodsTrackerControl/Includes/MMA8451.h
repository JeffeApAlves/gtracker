/*
* MMA8451.h
*    Author: Erich Styger
*/

#ifndef MMA8451_H_
#define MMA8451_H_

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"

#include "Telemetria.h"

#define LEN_XYZ		6

// Endereço dos registradores do MMA8451
#define MMA8451_STATUS                              0x00U
#define MMA8451_OUT_X_MSB							0x01U
#define MMA8451_OUT_X_LSB							0x02U
#define MMA8451_OUT_Y_MSB							0x03U
#define MMA8451_OUT_Y_LSB							0x04U
#define MMA8451_OUT_Z_MSB							0x05U
#define MMA8451_OUT_Z_LSB							0x06U
#define MMA8451_SYSMOD                              0x0BU
#define MMA8451_INT_SOURCE                          0x0CU
#define MMA8451_WHOAMI                              0x0DU
#define MMA8451_XYZ_DATA_CFG						0x0EU
#define MMA8451_PL_STATUS                           0x10U
#define MMA8451_PL_CFG                              0x11U
#define MMA8451_PL_COUNT                            0x12U // Orientation debounce
#define MMA8451_PL_BF_ZCOMP                         0x13U
#define MMA8451_PL_THS_REG                          0x14U
#define MMA8451_FF_MT_SRC                           0x16U
#define MMA8451_TRANSIENT_CFG                       0x1DU // Transient enable
#define MMA8451_TRANSIENT_SRC                       0x1EU // Transient read/clear interrupt
#define MMA8451_TRANSIENT_THS                       0x1FU // Transient threshold
#define MMA8451_TRANSIENT_COUNT                     0x20U // Transient debounce
#define MMA8451_PULSE_SRC                           0x22U
#define MMA8451_CTRL_REG1                           0x2AU
#define MMA8451_CTRL_REG2                           0x2BU
#define MMA8451_CTRL_REG3                           0x2CU // Interrupt control
#define MMA8451_CTRL_REG4                           0x2DU // Interrupt enable
#define MMA8451_CTRL_REG5                           0x2EU // Interrupt pin selection

/* MMA8451 3-axis accelerometer control register bit masks */
#define MMA8451_ACTIVE_BIT 		0x01
#define MMA8451_F_READ_BIT 		0x02
#define MMA8451_ZYXDR_BIT		0x08
#define MMA8451_L_NOISE_BIT		0x04

//REG2
#define MMA8451_MODS_MASK		0x03
#define MMA8451_MODS0_BIT		0x01
#define MMA8451_MODS1_BIT		0x02

#define MMA8451_PL_PUF            0
#define MMA8451_PL_PUB            1
#define MMA8451_PL_PDF            2
#define MMA8451_PL_PDB            3
#define MMA8451_PL_LRF            4
#define MMA8451_PL_LRB            5
#define MMA8451_PL_LLF            6
#define MMA8451_PL_LLB            7

typedef enum{

  MMA8451_RANGE_8_G           = 0b10,   // +/- 8g
  MMA8451_RANGE_4_G           = 0b01,   // +/- 4g
  MMA8451_RANGE_2_G           = 0b00    // +/- 2g (default value)
} mma8451_range_t;
#define MMA8451_RANGE_MASK	  0b11


/* Used with register 0x2A (MMA8451_CTRL_REG1) to set bandwidth */
typedef enum{

  MMA8451_DATARATE_800_HZ     = 0b000, //  800Hz
  MMA8451_DATARATE_400_HZ     = 0b001, //  400Hz
  MMA8451_DATARATE_200_HZ     = 0b010, //  200Hz
  MMA8451_DATARATE_100_HZ     = 0b011, //  100Hz
  MMA8451_DATARATE_50_HZ      = 0b100, //   50Hz
  MMA8451_DATARATE_12_5_HZ    = 0b101, // 12.5Hz
  MMA8451_DATARATE_6_25_HZ    = 0b110, // 6.25Hz
  MMA8451_DATARATE_1_56_HZ    = 0b111, // 1.56Hz

  MMA8451_DATARATE_MASK       = 0b111
} mma8451_dataRate_t;

bool MMA845x_getXYZ(Accelerometer* acc);
void MMA845x_deInit(void);
bool MMA845x_init(void);

/* Configura o acelerometro */
bool MMA845x_Config(void);

/* Ativa o acelerometro */
void MMA845x_Active(void);

/* Desativa o acelerometro */
void MMA845x_Standby(void);

/*REseta o acelerometro*/
bool MMA845x_Reset(void);

/* Le o vaalor de orientacao*/
uint8_t MMA845x_getOrientation( void );

/* COnfigura a faixa do acelerometro*/
void MMA845x_setRange(mma8451_range_t range);

/* Le a faixa do acelerometro*/
bool MMA845x_getRange(mma8451_range_t* range);

#define i2c_write(addr,val)				I2C_Write(&MMA8451_device,addr,val)
#define i2c_read(addr,val)				I2C_Read(&MMA8451_device,addr,val)
#define i2c_read_buffer(addr,data,size)	I2C_ReadBuffer(&MMA8451_device, addr, data,size)
#define i2c_init()						I2C_Init(&MMA8451_device)
#define i2c_whoAmI()					I2C_WhoAmI(&MMA8451_device,MMA8451_WHOAMI)
#define i2c_deinit()					I2C_Deinit(&MMA8451_device)


#endif /* MMA8451_H_ */
