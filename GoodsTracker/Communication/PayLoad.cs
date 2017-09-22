namespace GoodsTracker
{
    class PayLoad
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
            data += (b ? 1 : 0).ToString();
        }

        internal void Append(double b)
        {
            data += b.ToString("G");
        }

        internal int Length()
        {
            return data == null ? 0 : data.Length;
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

        internal void setData(CommunicationFrame frame)
        {
            string value = frame.Data;

            if (value.Length > (HeaderFrame.SIZE + 1))
            {
                data = value.Substring((HeaderFrame.SIZE + 1), value.Length - (HeaderFrame.SIZE + 1));
            }
        }

        internal char[] ToCharArray()
        {
            return str().ToCharArray();
        }
    }
}
