#include "MMA8451.h"
#include "I2C2.h"

static MMA8451_TDataState deviceData;
static int8_t xyz[LEN_XYZ];

uint8_t ReadReg(uint8_t addr, uint8_t *data, short dataSize) {

  uint8_t res;

  /* Send I2C address plus register address to the I2C bus *without* a stop condition */
  res = I2C2_MasterSendBlock(deviceData.handle, &addr, 1U, LDD_I2C_NO_SEND_STOP);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataTransmittedFlg) {} /* Wait until data is sent */
  deviceData.dataTransmittedFlg = FALSE;

  /* Receive InpData (1 byte) from the I2C bus and generates a stop condition to end transmission */
  res = I2C2_MasterReceiveBlock(deviceData.handle, data, dataSize, LDD_I2C_SEND_STOP);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataReceivedFlg) {} /* Wait until data is received received */
  deviceData.dataReceivedFlg = FALSE;
  return ERR_OK;
}
//--------------------------------------------------------------------------------------------

uint8_t WriteReg(uint8_t addr, uint8_t val) {
  uint8_t buf[2], res;

  buf[0] = addr;
  buf[1] = val;
  res = I2C2_MasterSendBlock(deviceData.handle, &buf, 2U, LDD_I2C_SEND_STOP); /* Send OutData (3 bytes with address) on the I2C bus and generates not a stop condition to end transmission */
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataTransmittedFlg) {}  /* Wait until date is sent */
  deviceData.dataTransmittedFlg = FALSE;
  return ERR_OK;
}
//--------------------------------------------------------------------------------------------

void MMA845x_Run(void) {

	int16_t x, y, z;
	float x_g, y_g, z_g;
	uint8_t res;

	deviceData.handle = I2C2_Init(&deviceData);

	/* F_READ: Fast read mode, data format limited to single byte (auto increment counter will skip LSB)
	 * ACTIVE: Full scale selection
	 */
	res = WriteReg(MMA8451_CTRL_REG_1,  MMA8451_F_READ_BIT_MASK | MMA8451_ACTIVE_BIT_MASK);

	if (res==ERR_OK) {

		for(;;) {
			res = ReadReg(MMA8451_OUT_X_MSB, (uint8_t*)&xyz,LEN_XYZ);
		}
	}

	I2C2_Deinit(deviceData.handle);
}
//--------------------------------------------------------------------------------------------

bool MMA845x_getValues(int* axis){

	tword x,y,z;

	uint8_t res = ERR_FAILED;
	uint8_t status;

	res = ReadReg(MMA8451_STATUS_00_REG,&status,1);

	if ((res==ERR_OK) && (status & MMA8451_ZYXDR_BIT_MASK)){

		res = ReadReg(MMA8451_OUT_X_MSB, (uint8_t*)&xyz,LEN_XYZ);
/*
		if(res==ERR_OK){

			axis[0] = ( xyz[0] << 8  );
			axis[1] = ( xyz[1] << 8  );
			axis[2] = ( xyz[2] << 8  );
		}*/


		if(res==ERR_OK){

			x.Byte.hi	= xyz[0];
			x.Byte.low	= xyz[1];
			y.Byte.hi	= xyz[2];
			y.Byte.low	= xyz[3];
			z.Byte.hi	= xyz[4];
			z.Byte.low	= xyz[5];

			convertDecimal (&x);
			convertDecimal (&y);
			convertDecimal (&z);

			axis[0] = x.Word;
			axis[1] = y.Word;
			axis[2] = z.Word;

/*
			axis[0] = ((( xyz[0] << 8  ) | (xyz[1] & 0x00FF ))>>2) & 0x3FFF;
			axis[1] = ((( xyz[2] << 8  ) | (xyz[3] & 0x00FF ))>>2) & 0x3FFF;
			axis[2] = ((( xyz[4] << 8  ) | (xyz[5] & 0x00FF ))>>2) & 0x3FFF;

			if(xyz[0]>0x7F){
				axis[0] |= 0x8000;
			}

			if(xyz[2]>0x7F){
				axis[1] |= 0x8000;
			}

			if(xyz[4]>0x7F){
				axis[2] |= 0x8000;
			}
			*/
		}
	}

	return res == ERR_OK;
}
//--------------------------------------------------------------------------------------------

void MMA845x_Standby (void)
{
	/*
	** Read current value of System Control 1 Register.
	** Put sensor into Standby Mode by clearing the Active bit
	** Return with previous value of System Control 1 Register.
	*/
	uint8_t ctrl;

	ReadReg(MMA8451_CTRL_REG_1, &ctrl, 1);

	WriteReg(MMA8451_CTRL_REG_1, ctrl & (~MMA8451_ACTIVE_BIT_MASK));
}
//--------------------------------------------------------------------------------------------

void MMA845x_Active(void)
{
	uint8_t ctrl;

	ReadReg(MMA8451_CTRL_REG_1, &ctrl, 1);

	WriteReg(MMA8451_CTRL_REG_1, ctrl | MMA8451_ACTIVE_BIT_MASK);
}
//--------------------------------------------------------------------------------------------


void MMA845x_init(void){

	uint8_t ctrl;


	deviceData.handle = I2C2_Init(&deviceData);

	MMA845x_Standby();

	ReadReg(MMA8451_CTRL_REG_1,&ctrl,1);

	WriteReg(MMA8451_CTRL_REG_1,ctrl & (~MMA8451_F_READ_BIT_MASK));

	MMA845x_Active();
}
//--------------------------------------------------------------------------------------------

void MMA845x_deInit(void){

	I2C2_Deinit(deviceData.handle);
}
//--------------------------------------------------------------------------------------------

void convertDecimal (tword* data)
{
	char out[6];
/*
 * 				0xFAB1	= -1339
	Example:	0xABCC = -1349
				0x5443 = +1349*/

//	data->Word = 0xABCC;

	byte a, b, c, d;
	word r;
	/*
	** Determine sign and output
	*/
	if (data->Byte.hi > 0x7F){

		//SCI_CharOut ('-');
		out[0] = '-';
		data->Word = (~data->Word) + 1;

	}else{

		//SCI_CharOut ('+');
		out[0] = '+';
	}
	/*
	** Calculate decimal equivalence:
	** a = thousands
	** b = hundreds
	** c = tens
	** d = ones
	*/
	a = (byte)((data->Word >>2) / 1000);
	r = (data->Word >>2) % 1000;
	b = (byte)(r / 100);
	r %= 100;
	c = (byte)(r / 10);
	d = (byte)(r % 10);
	/*
	**
	*/
	if (a == '0'){

		a = 0xF0;

		if (b == '0'){

			b = 0xF0;
			if (c == '0'){

				c = 0xF0;
			}
		}
	}
	/*
	* Output result
	*/

	out[1] = a;
	out[2] = b;
	out[3] = c;
	out[4] = d;
	out[5] = '\0';

//	SCI_NibbOut (a);
//	SCI_NibbOut (b);
//	SCI_NibbOut (c);
//	SCI_NibbOut (d);
}
//--------------------------------------------------------------------------------------------
