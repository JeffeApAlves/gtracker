using System.Text;

namespace GoodsTracker
{
    /**
     * 
     * Frame Coomunication
     * [ End. de orig[5] : End dest[5] : Count frame[5] : Operacao[2] : Recurso[3] : SizePayload[3] : payload[ 0 ~ 255] : CheckSum[2] ] \r\n
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

    interface IDecoderFrame
    {
        bool setValues(out PayLoad payload, DataTelemetria b);
        bool getValues(out AnsCmd ans, DataFrame frame);
    }

    public class RESOURCE
    {
        public const string TLM         = "TLM";    // Data DataTelemetria
        public const string LOCK        = "LCK";    // Trava 
        public const string LCD         = "LCD";
    }

    public enum Operation
    {
        NN,RD, WR, AN
    }

    internal class Header
    {
        public const int SIZE = 27;             // 5+5+5+2+3+3 + 4 separadores

        string data;
        int dest;
        int address;
        int count = 0;
        Operation operation;
        string resource;
        int sizePayLoad;

        public int Dest { get => dest; set => dest = value; }
        public int Address { get => address; set => address = value; }
        public Operation Operation { get => operation; set => operation = value; }
        public string Resource { get => resource; set => resource = value; }
        public int SizePayLoad { get => sizePayLoad; set => sizePayLoad = value; }
        public int Count { get => count; set => count = value; }

        internal Header()
        {
            count = 0;
            address = 0;
            dest = 0;
            resource = "";
            operation = Operation.NN;
            sizePayLoad = 0;
        }

        internal string str()
        {
            data = "";

            DecoderFrame.setHeader(this);

            return data;
        }

        internal void Clear()
        {
            data = "";
        }

        internal void Append(char b)
        {
            data += b;
        }

        internal void Append(string b)
        {
            data += b;
        }

        internal void Append(double b)
        {
            data += b.ToString("G");
        }

        internal void setData(DataFrame frame)
        {
            string value = frame.Data;

            if (value.Length > Header.SIZE)
            {
                data = value.Substring(0, Header.SIZE);
            }
            else
            {
                data = value;
            }
        }

        internal char[] ToCharArray()
        {
            return str().ToCharArray();
        }
    }

    internal class PayLoad
    {
        public const int    LEN_MAX_PAYLOAD = 256;
        private string      data;

        internal string Data { get => data; set => data = value; }

        internal void Append(char b)
        {
            data += b;
        }

        internal void Append(string b)
        {
            data += b;
        }

        internal void Append(bool b)
        {
            data += (b?1:0).ToString();
        }


        internal void Append(double b)
        {
            data += b.ToString("G");
        }

        internal int Length()
        {
            return data==null? 0:data.Length;
        }

        internal bool IsFull()
        {
            return Length() >= LEN_MAX_PAYLOAD;
        }

        internal bool IsEmpty()
        {
            return Length() <= 0;
        }

        internal string str()
        {
            return Length().ToString("D3") + CONST_CHAR.SEPARATOR + data + CONST_CHAR.SEPARATOR;
        }

        internal void Clear()
        {
            data = "";
        }

        internal void setData(DataFrame frame)
        {
            string value = frame.Data;

            if (value.Length > (Header.SIZE + 1))
            {
                data = value.Substring((Header.SIZE + 1), value.Length - (Header.SIZE + 1));
            }
        }

        internal char[] ToCharArray()
        {
            return str().ToCharArray();
        }

    }

    internal class DataFrame
    {
        protected Header    header;
        protected PayLoad   payLoad;
        protected string    data;

        public Header Header
        {
            get
            {
                return header;
            }

            set
            {
                header = value;
                data = header.str() + CONST_CHAR.SEPARATOR + (payLoad==null?"":payLoad.str());
            }
        }

        public PayLoad PayLoad
        {
            get
            {
                return payLoad;
            }

            set
            {
                payLoad = value;
                data    = header.str() + CONST_CHAR.SEPARATOR + (payLoad == null ? "" : payLoad.str());
            }
        }

        public string Data
        {
            get
            {
                return data;
            }

            set
            {
                data   = value;
                header.setData(this);
                payLoad.setData(this);
            }
        }

        internal DataFrame(Header h,PayLoad p)
        {
            data    = "";
            Header  = h;
            PayLoad = p;
        }

        internal DataFrame()
        {
            header  = new Header();
            payLoad = new PayLoad();
            data   = "";
        }

        internal byte getByte(int i)
        {
            return (byte)data[i];
        }

        internal void putByte(char b)
        {
            Data += b;
        }

        internal int getSizeOfFrame()
        {
            return data==null?0:data.Length;
        }

        internal bool isFrameEmpty()
        {
            return getSizeOfFrame() <= 0;
        }

        internal byte checkSum()
        {
            byte checkSum = 0;

            byte[] datas = Encoding.ASCII.GetBytes(data);

            for (int i = 0; i < datas.Length; i++)
            {
                checkSum ^= datas[i];
            }

            return checkSum;
        }

        internal string str()
        {
            return  CONST_CHAR.RX_FRAME_START +
                    data +
                    checkSum().ToString("X2") +
                    CONST_CHAR.RX_FRAME_END;
        }

        internal char[] ToCharArray()
        {
            return  str().ToCharArray();
        }
    }
}
