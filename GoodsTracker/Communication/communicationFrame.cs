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
     * [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[2] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ] \r\n
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
     * Range: 01~99
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

    interface IDecoderFrameTx
    {
        bool setFrame(out CommunicationFrame frame, CommunicationUnit unit);

        bool setPayLoad(ref CommunicationFrame frame, Behavior b);
    }

    interface IDecoderFrameRx
    {
        bool getValues(out ObjectValueRX dadosRx, CommunicationFrame frame);
    }

    public struct ObjectValueRXAxis
    {
        public double acceleration;
        public double rotation;
    };

    public struct ObjectValueRX
    {
        public int orig;
        public int dest;
        public string operation;
        public int resource;
        public int size;
        public double latitude;
        public double longitude;
        public ObjectValueRXAxis X, Y, Z;
        public double Level;
        public int checksum;
        public int speed;
    };


    internal class CommunicationFrame
    {
        public const int LEN_MAX_PAYLOAD = 256;
        protected string payLoad;
        protected string header;
        protected string frame;

        public string Header
        {
            get
            {
                return header;
            }

            set
            {
                header = value;
                frame = header + frame;
            }
        }

        public string PayLoad
        {
            get
            {
                return payLoad;
            }

            set
            {
                payLoad = value;
                frame = header + frame;
            }
        }

        public string Frame
        {
            get
            {
                return frame;
            }

            set
            {
                frame = value;
            }
        }

        internal CommunicationFrame()
        {
            clear();
        }

        internal byte getByteOfFrame(int i)
        {
            return (byte)frame[i];
        }
/*
        internal void setByteOfFrame(int i, byte b)
        {
            char[] letters  = payLoad.ToCharArray();
            letters[i]      = (char)b;
            frame           = string.Join("", letters);
        }
*/
        internal void addByteInFrame(byte b)
        {
            frame = string.Join(frame,(char)b);
        }

        internal void clear()
        {
            header  = "";
            payLoad = "";
            frame   = "";
        }

        internal int getSizeOfPayLoad()
        {
            return payLoad.Length;
        }

        internal int getSizeOfFrame()
        {
            return frame.Length;
        }

        internal bool isPayloadFull()
        {
            return getSizeOfPayLoad() >= LEN_MAX_PAYLOAD;
        }

        internal bool isPayLoadEmpty()
        {
            return getSizeOfPayLoad() <= 0;
        }

        internal int checkSum()
        {
            int checkSum = 0;

            for (int i = 0; i < payLoad.Length; i++)
            {
                checkSum += getByteOfFrame(i);
            }

            return checkSum;
        }
    }
}
