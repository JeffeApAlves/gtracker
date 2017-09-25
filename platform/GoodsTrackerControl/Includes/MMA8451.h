/*
* MMA8451.h
*    Author: Erich Styger
*/

#ifndef MMA8451_H_
#define MMA8451_H_

#include <stdint.h>

#include "PE_LDD.h"

#include "Telemetria.h"

#define LEN_XYZ		6

#define MMA8451_STATUS                               0x00 //
#define MMA8451_OUT_X_MSB                            0x01 //
#define MMA8451_SYSMOD                               0x0B //
#define MMA8451_INT_SOURCE                           0x0C //
#define MMA8451_ID                                   0x0D //
#define MMA8451_XYZ_DATA_CFG						 0x0E
#define MMA8451_PL_STATUS                            0x10 //
#define MMA8451_PL_CFG                               0x11 //
#define MMA8451_PL_COUNT                             0x12 // Orientation debounce
#define MMA8451_PL_BF_ZCOMP                          0x13 //
#define MMA8451_PL_THS_REG                           0x14 //
#define MMA8451_FF_MT_SRC                            0x16 //
#define MMA8451_TRANSIENT_CFG                        0x1D // Transient enable
#define MMA8451_TRANSIENT_SRC                        0x1E // Transient read/clear interrupt
#define MMA8451_TRANSIENT_THS                        0x1F // Transient threshold
#define MMA8451_TRANSIENT_COUNT                      0x20 // Transient debounce
#define MMA8451_PULSE_SRC                            0x22 //
#define MMA8451_CTRL_REG1                            0x2A //
#define MMA8451_CTRL_REG2                            0x2B //
#define MMA8451_CTRL_REG3                            0x2C // Interrupt control
#define MMA8451_CTRL_REG4                            0x2D // Interrupt enable
#define MMA8451_CTRL_REG5                            0x2E // Interrupt pin selection



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


/* External 3-axis accelerometer data register addresses */
#define MMA8451_OUT_X_MSB 0x01
#define MMA8451_OUT_X_LSB 0x02
#define MMA8451_OUT_Y_MSB 0x03
#define MMA8451_OUT_Y_LSB 0x04
#define MMA8451_OUT_Z_MSB 0x05
#define MMA8451_OUT_Z_LSB 0x06

typedef struct {
  volatile bool dataReceivedFlg; /* set to TRUE by the interrupt if we have received data */
  volatile bool dataTransmittedFlg; /* set to TRUE by the interrupt if we have set data */
  LDD_TDeviceData *handle; /* pointer to the device handle */
} MMA8451_TDataState;

uint8_t I2C_ReadBuffer(uint8_t addr, uint8_t *data, short dataSize);
uint8_t I2C_Read(uint8_t addr, uint8_t *data);
uint8_t I2C_Write(uint8_t addr, uint8_t val);

bool MMA845x_getXYZ(Accelerometer* acc);
void MMA845x_deInit(void);
void MMA845x_init(void);
void MMA845x_Standby(void);
void MMA845x_Active (void);
uint8_t MMA845x_Reset(void);
uint8_t MMA845x_getOrientation( void );
void MMA845x_setRange(mma8451_range_t range);


mma8451_range_t MMA845x_getRange(void);

short toDecimal (int8_t* hi_lo);

#endif /* MMA8451_H_ */
