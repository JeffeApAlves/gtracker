
#include "MMA8451.h"

/* converte o valor para decimal com sinal*/
static int16_t toDecimal (uint8_t* hi_lo);

static i2c_state_t MMA8451_device;


/**
 *
 * Realiza a leitura de todos os eixos
 */
bool MMA845x_getXYZ(Accelerometer* acc){

	uint8_t		xyz[LEN_XYZ+1];
	bool		res		= false;
	uint8_t		status	= 0;

	mma8451_range_t range =  MMA8451_RANGE_4_G;

	if(i2c_read(MMA8451_STATUS,&status) && (status & MMA8451_ZYXDR_BIT)){

		if(i2c_read_buffer(MMA8451_STATUS,xyz,LEN_XYZ+1)){

			MMA845x_getRange(&range);

			//status = xyz[0];

			acc->x = toDecimal(xyz+1);
			acc->y = toDecimal(xyz+3);
			acc->z = toDecimal(xyz+5);

			float divider = 1.0;
			if (range == MMA8451_RANGE_8_G) divider = 1024.0;
			if (range == MMA8451_RANGE_4_G) divider = 2048.0;
			if (range == MMA8451_RANGE_2_G) divider = 4096.0;

			acc->x_g = (float)acc->x / divider;
			acc->y_g = (float)acc->y / divider;
			acc->z_g = (float)acc->z / divider;

			//PRINTF("status_reg = 0x%x , x = %f , y = %f , z = %f \r\n", status, acc->x_g, acc->y_g, acc->z_g);

			res = true;
		}
	}

	return res;
}
//--------------------------------------------------------------------------------------------

bool MMA845x_init(void){

	i2c_init();

    if(i2c_whoAmI()){

    	if(MMA8451_device.identification == MMA8451_ID){

    		if(MMA845x_Reset()){

				if(MMA845x_Config()){

					//PRINTF("[INFO]:Acelerometro MMA8451 configurado com sucesso\r\n");

					return true;
				}
    		}else{

    			//PRINTF("[ERROR]:Nao foi possivel reiniciar o acelerometro\r\n");
    		}
    	}
    }

    return false;
}
//--------------------------------------------------------------------------------------------

void inline MMA845x_deInit(void){
}
//--------------------------------------------------------------------------------------------

void MMA845x_Standby (void){

	uint8_t ctrl;

	i2c_read(MMA8451_CTRL_REG1, &ctrl);
	i2c_write(MMA8451_CTRL_REG1, ctrl & (~MMA8451_ACTIVE_BIT));
}
//--------------------------------------------------------------------------------------------

void MMA845x_Active(void){

	uint8_t ctrl;

	i2c_read(MMA8451_CTRL_REG1, &ctrl);
	i2c_write(MMA8451_CTRL_REG1, ctrl | MMA8451_ACTIVE_BIT);
}
//--------------------------------------------------------------------------------------------

bool MMA845x_Config(void){

	bool ok_config = false;

	uint8_t ctrl_reg1=0,ctrl_reg2=0;

	MMA845x_Standby();

	// Espelha registradores
	if(i2c_read(MMA8451_CTRL_REG1,&ctrl_reg1) && i2c_read(MMA8451_CTRL_REG2,&ctrl_reg2)){

		// High Resolution
		ctrl_reg2 &= ~MMA8451_MODS_MASK;
		ctrl_reg2 |= MMA8451_MODS1_BIT;

		// Hi e Lo indexados
		ctrl_reg1 &= (~MMA8451_F_READ_BIT);

		//Low noise
		ctrl_reg1 |= MMA8451_L_NOISE_BIT;

		// Date rate = 50Hz (20 ms)
		ctrl_reg1 &= ~(MMA8451_DATARATE_MASK << 3);
		ctrl_reg1 |= (MMA8451_DATARATE_50_HZ << 3);

				// Atualiza reg 1
		if(		i2c_write(MMA8451_CTRL_REG1,ctrl_reg1) &&
				// Atualiza reg 2
				i2c_write(MMA8451_CTRL_REG2,ctrl_reg2) &&
				// Range
				i2c_write(MMA8451_XYZ_DATA_CFG,  MMA8451_RANGE_4_G)){

			ok_config = true;
		}
	}

	MMA845x_Active();

	return ok_config;
}
//--------------------------------------------------------------------------------------------

bool MMA845x_getRange(mma8451_range_t* range){

	uint8_t data_cfg;

	if(i2c_read(MMA8451_XYZ_DATA_CFG, &data_cfg)){

		*range =  (mma8451_range_t)(data_cfg  & MMA8451_RANGE_MASK);

		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------------

bool MMA845x_Reset(void)
{
//	uint8_t ctrl_reg2 = 0;

//	if(i2c_write(MMA8451_CTRL_REG2, 0x40 )){

//
//		do{
//			i2c_read(MMA8451_CTRL_REG2,&ctrl_reg2);
//		}while (ctrl_reg2 & 0x40);

		return true;
//	}

//	return false;
}
//--------------------------------------------------------------------------------------------

uint8_t MMA845x_getOrientation( void ){

    uint8_t orientation = 0;

    i2c_read(MMA8451_PL_STATUS, &orientation);

    return orientation;
}
//--------------------------------------------------------------------------------------------

void MMA845x_setDataRate(mma8451_dataRate_t dataRate){

	uint8_t ctrlReg1;

	if(i2c_read(MMA8451_CTRL_REG1,&ctrlReg1)){

		MMA845x_Standby();

		ctrlReg1 &= ~(MMA8451_DATARATE_MASK << 3);
		ctrlReg1 |= (dataRate << 3);

		i2c_write(MMA8451_CTRL_REG1,ctrlReg1);

		MMA845x_Active();
	}
}
//--------------------------------------------------------------------------------------------

void MMA845x_setNoise(bool flag){

	uint8_t ctrlReg1;

	if(i2c_read(MMA8451_CTRL_REG1,&ctrlReg1)){

		MMA845x_Standby();

		ctrlReg1 &= ~MMA8451_L_NOISE_BIT;

		if(flag){

			ctrlReg1 |= MMA8451_L_NOISE_BIT;
		}

		i2c_write(MMA8451_CTRL_REG1,ctrlReg1);

		MMA845x_Active();
	}
}
//--------------------------------------------------------------------------------------------

void MMA845x_setRange(mma8451_range_t range){

	uint8_t cfg;

	MMA845x_Standby();

	i2c_read(MMA8451_XYZ_DATA_CFG, &cfg);

	cfg &= (~MMA8451_RANGE_MASK);

	i2c_write(MMA8451_XYZ_DATA_CFG, cfg | (range &  MMA8451_RANGE_MASK));

	MMA845x_Active();
}
//--------------------------------------------------------------------------------------------

static inline int16_t toDecimal (uint8_t* hi_lo){

	return ((int16_t)((hi_lo[0]<<8) | hi_lo[1]))>>2;
}
//--------------------------------------------------------------------------------------------
