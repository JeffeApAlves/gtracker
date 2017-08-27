/*
* MMA8451.h
*    Author: Erich Styger
*/

#ifndef MMA8451_H_
#define MMA8451_H_

#include "PE_Types.h"
#include "PE_LDD.h"

typedef struct {

	union{

		struct{
			byte low;
			byte hi;
		}Byte;

		word Word;
	};

} tword;

/* External 3-axis accelerometer control register addresses */
#define MMA8451_CTRL_REG_1 0x2A
#define MMA8451_STATUS_00_REG 0x00

/* MMA8451 3-axis accelerometer control register bit masks */
#define MMA8451_ACTIVE_BIT_MASK 0x01
#define MMA8451_F_READ_BIT_MASK 0x02

#define MMA8451_ZYXDR_BIT_MASK	0x08

/* External 3-axis accelerometer data register addresses */
#define MMA8451_OUT_X_MSB 0x01
#define MMA8451_OUT_X_LSB 0x02
#define MMA8451_OUT_Y_MSB 0x03
#define MMA8451_OUT_Y_LSB 0x04
#define MMA8451_OUT_Z_MSB 0x05
#define MMA8451_OUT_Z_LSB 0x06

#define LEN_XYZ		6

typedef struct {
  volatile bool dataReceivedFlg; /* set to TRUE by the interrupt if we have received data */
  volatile bool dataTransmittedFlg; /* set to TRUE by the interrupt if we have set data */
  LDD_TDeviceData *handle; /* pointer to the device handle */
} MMA8451_TDataState;

void MMA845x_Run(void);
bool MMA845x_getValues(int* axis);
void MMA845x_deInit(void);
void MMA845x_init(void);
void MMA845x_Standby(void);
void MMA845x_Active (void);
void convertDecimal(tword* data);

#endif /* MMA8451_H_ */
