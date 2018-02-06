using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    /**
     * 
     * Frame Coomunication
     * [ End. de orig[5] : End dest[5] : tIME STAMP fr[5] : Operacao[2] : Recurso[3] : SizePayload[3] : payload[ 0 ~ 255] : CheckSum[2] ] \r\n
     * 
     * End. de orig: 
     * Range: 00000~65535 (00000) Broadcast
     * 
     * End. de dest: 
     * Range: 00000~65535 (00000) Broadcast
     * 
     * Operacao:
     * Possiveis: 
     * RD = READ
     * WR = WRITE
     * AN + ANSWER
     * 
     * Recurso: 
     * Range: A-Z a-z 0~9
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

    internal enum INDEX
    {
        ADDRESS = 0,
        DEST = 1,
        TIME_STAMP = 2,
        OPERACAO = 3,
        RESOURCE = 4,
        SIZE_PAYLOAD = 5,
        LAT = 6,
        LNG = 7,
        ACCEL_X = 8,
        ACCEL_Y = 9,
        ACCEL_Z = 10,
        ACCEL_XG = 11,
        ACCEL_YG = 12,
        ACCEL_ZG = 13,
        SPEED = 14,
        LEVEL = 15,
        TRAVA = 16,
        TIME_STAMP_PL = 17,
    };

    class FrameSerial : CommunicationFrame
    {
        internal FrameSerial(HeaderFrame h, PayLoad p)
            :base(h,p)
        {
            
        }

        internal FrameSerial()
            : base()
        {

        }
    }
}
