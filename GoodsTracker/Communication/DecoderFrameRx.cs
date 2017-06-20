﻿using System;

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
        enum DATA1 {

            ORIG    = 0,
            DEST    = 1,

            LAT     = 5,
            LNG     = 6,
            ACCEL_X = 7,
            ACCEL_Y = 8,
            ACCEL_Z = 9,
            LEVEL   = 10,
            SPEED   = 11,
        }

        public bool getValues(out ObjectValueRX dadosRx, CommunicationFrame frame)
        {
            bool ret    = false;
            dadosRx     = new ObjectValueRX();

            try
            {
                string[] list   = frame.Frame.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 9)
                {
                    setVal(ref dadosRx.latitude,        list, (int)DATA1.LAT);
                    setVal(ref dadosRx.longitude,       list, (int)DATA1.LNG);
                    setVal(ref dadosRx.longitude,       list, (int)DATA1.ACCEL_X);
                    setVal(ref dadosRx.longitude,       list, (int)DATA1.ACCEL_Y);
                    setVal(ref dadosRx.X.acceleration,  list, (int)DATA1.ACCEL_X);
                    setVal(ref dadosRx.Y.acceleration,  list, (int)DATA1.ACCEL_Y);
                    setVal(ref dadosRx.Z.acceleration,  list, (int)DATA1.ACCEL_Z);
                    setVal(ref dadosRx.level,           list, (int)DATA1.LEVEL);
                    setVal(ref dadosRx.speed,           list, (int)DATA1.SPEED);

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