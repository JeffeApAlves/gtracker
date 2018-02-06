namespace GoodsTracker
{
    class HeaderFrame
    {
        public const int SIZE = 27;             // 5+5+5+2+3+3 + 4 separadores

        string      data;
        int         dest;
        int         address;
        int         timeStamp = 0;
        Operation   operation;
        string      resource;
        int         sizePayLoad;

        public int Dest { get => dest; set => dest = value; }
        public int Address { get => address; set => address = value; }
        public Operation Operation { get => operation; set => operation = value; }
        public string Resource { get => resource; set => resource = value; }
        public int SizePayLoad { get => sizePayLoad; set => sizePayLoad = value; }
        public int TimeStamp { get => timeStamp; set => timeStamp = value; }

        internal HeaderFrame()
        {
            timeStamp = 0;
            address = 0;
            dest = 0;
            resource = "";
            operation = Operation.NN;
            sizePayLoad = 0;
        }

        internal string str()
        {
            data = "";

            SerialSerialization decoder = new SerialSerialization();

            decoder.encode(this);

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

        internal void setData(CommunicationFrame frame)
        {
            string value = frame.Data;

            if (value.Length > SIZE)
            {
                data = value.Substring(0, SIZE);
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
}
