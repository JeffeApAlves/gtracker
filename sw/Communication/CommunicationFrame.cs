using System.Text;

namespace GoodsTracker
{
    public struct RESOURCE
    {
        public const string TLM = "TLM";    // Data DataTelemetria
        public const string LCK = "LCK";    // Trava 
        public const string LCD = "LCD";
    }

    public enum Operation
    {
        NN,RD, WR, AN
    }

    class CommunicationFrame
    {
        protected HeaderFrame   header;
        protected PayLoad       payload;
        protected string        data;
        protected FrameSerialization serialization=null;

        public HeaderFrame Header
        {
            get
            {
                return header;
            }

            set
            {
                header = value;
                data = header.str() + CONST_CHAR.SEPARATOR + (payload==null?"":payload.str());
            }

        }

        public PayLoad PayLoad
        {
            get
            {
                return payload;
            }

            set
            {
                payload = value;
                data    = header.str() + CONST_CHAR.SEPARATOR + (payload == null ? "" : payload.str());
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
                payload.setData(this);
            }
        }

        internal CommunicationFrame(HeaderFrame h,PayLoad p)
        {
            data    = "";
            Header  = h;
            PayLoad = p;
            serialization = Communication.createSerialization();
        }

        internal CommunicationFrame()
        {
            header  = new HeaderFrame();
            payload = new PayLoad();
            data    = "";
            serialization = Communication.createSerialization();
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

        /*
         * Retorna o frame completo
         * 
         */
        public string str()
        {
            return  CONST_CHAR.RX_FRAME_START +
                    data +
                    checkSum().ToString("X2") +
                    CONST_CHAR.RX_FRAME_END;
        }

        /*
         * Retorna o frame completo
         * 
         */
        internal char[] ToCharArray()
        {
            return  str().ToCharArray();
        }

        internal void encode(AnsCmd ans)
        {
            PayLoad pl;

            if (ans.Header.Resource.Equals(RESOURCE.TLM))
            {
                serialization.encode(out pl, ans.Telemetria);
            }
            else
            {
                pl = new PayLoad();
            }

            Header      = ans.Header;
            PayLoad     = pl;
        }
    }
}
