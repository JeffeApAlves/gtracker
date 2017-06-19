using System;

namespace GoodsTracker
{
    /**
     * 
     * Frame Rx
     * [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[5] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ]
     * 
     * End. de orig: 
     * Range: 00000~65535 (00000) Broadcast
     * 
     * End. de dest: 
     * Range: 00000~65535 (00000) Broadcast
     * 
     * Operacao:
     * Possiveis: RD ou WR
     * 
     * Recurso: 
     * Range: A~Z 0~9
     * 01: Lat , Long , AccelX , AccelY , AccelZ , Level, Speed
     * 
     * SizePayload:
     * Range: 0~255
     * 
     * Payload:
     * Conteudo App
     * Observacao: '[' ']' sao caracteres especiais entao usar \] e \[ 
     * 
     * CheckSum 
     * Somatoria
     */

    class DecoderFrameRx : IDecoderFrameRx
    {
        enum PALYLOAD_1 {

            LAT     = 0,
            LNG     = 1,
            ACCEL_X = 2,
            ACCEL_Y = 3,
            ACCEL_Z = 4,
            LEVEL   = 5,
            SPEED   = 6,
        }

        public bool getValues(out ObjectValueRX dadosRx, CommunicationFrame frame)
        {
            bool ret    = false;
            dadosRx     = new ObjectValueRX();

            try
            {
                string[] list   = frame.PayLoad.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 9)
                {
                    setVal(ref dadosRx.latitude,        list, (int)PALYLOAD_1.LAT);
                    setVal(ref dadosRx.longitude,       list, (int)PALYLOAD_1.LNG);
                    setVal(ref dadosRx.longitude,       list, (int)PALYLOAD_1.ACCEL_X);
                    setVal(ref dadosRx.longitude,       list, (int)PALYLOAD_1.ACCEL_Y);
                    setVal(ref dadosRx.X.acceleration,  list, (int)PALYLOAD_1.ACCEL_X);
                    setVal(ref dadosRx.Y.acceleration,  list, (int)PALYLOAD_1.ACCEL_Y);
                    setVal(ref dadosRx.Z.acceleration,  list, (int)PALYLOAD_1.ACCEL_Z);
                    setVal(ref dadosRx.Level,           list, (int)PALYLOAD_1.LEVEL);
                    setVal(ref dadosRx.speed,           list, (int)PALYLOAD_1.SPEED);

                    ret = true;
                }
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        private void setVal(ref double dest,string[] list,int index)
        {
            dest = 0;

            if (index < list.Length) {

                dest = Convert.ToDouble(list[index]);
            }
        }

        private void setVal(ref int dest, string[] list, int index)
        {
            dest = 0;

            if (index < list.Length)
            {
                dest = Convert.ToInt16(list[index]);
            }
        }
    }
}