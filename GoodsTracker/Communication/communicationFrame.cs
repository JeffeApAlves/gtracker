using System.Text;

namespace GoodsTracker
{
    /**
     * 
     * Frame Coomunication
     * [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[3] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ] \r\n
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
        bool setHeader(ref CommunicationFrame frame, CommunicationUnit unit,Cmd cmd);
        bool setHeader(ref CommunicationFrame frame, CommunicationUnit unit);
        bool setPayLoad(ref CommunicationFrame frame, TelemetriaData b);
    }

    interface IDecoderFrameRx
    {
        bool getValues(out AnsCmd ans, CommunicationFrame frame);
    }

    public enum Operation
    {
        RD, WR, AN
    }

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
                frame = header + payLoad;
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
                frame = header + payLoad;
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
                frame   = value;
                if (value.Length >= 15)
                {
                    Header = frame.Substring(0, 15);
                }
                else
                {
                    Header = frame;
                }

                if (value.Length >= 16)
                {
                    PayLoad = frame.Substring(15, frame.Length - 14);
                }
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

        internal void addByteInFrame(char b)
        {
            frame += b;
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

        internal bool isPayLoadFull()
        {
            return getSizeOfPayLoad() >= LEN_MAX_PAYLOAD;
        }

        internal bool isPayLoadEmpty()
        {
            return getSizeOfPayLoad() <= 0;
        }

        internal byte checkSum()
        {
            byte checkSum = 0;

            byte[] datas = Encoding.ASCII.GetBytes(frame);

            for (int i = 0; i < datas.Length; i++)
            {
                checkSum ^= datas[i];
            }

            return checkSum;
        }
    }
}
