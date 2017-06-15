using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class CommunicationFrame
    {
        const int LEN_MAX_PAYLOAD = 64;

        protected string payload;

        public CommunicationFrame()
        {
            payload = "";

            clear();
        }

        public byte getByte(int i)
        {
            return (byte)payload[i];
        }

        public void setByte(int i, byte b)
        {
            char[] letters  = payload.ToCharArray();
            letters[i]      = (char)b;
            payload         = string.Join("", letters);
        }

        public void addByte(byte b)
        {
            payload = string.Join(payload,(char)b);
        }

        public void clear()
        {
            payload = "";
        }

        public int getCount()
        {
            return payload.Length;
        }

        public bool isFull()
        {
            return getCount() >= LEN_MAX_PAYLOAD;
        }

        public bool isEmpty()
        {
            return getCount() <= 0;
        }

        public string getPayLoad()
        {
            return payload;
        }

        public void setPayLoad(string str)
        {
            payload = str;
        }
    }
}
