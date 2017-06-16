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

        internal CommunicationFrame()
        {
            payload = "";

            clear();
        }

        internal byte getByte(int i)
        {
            return (byte)payload[i];
        }

        internal void setByte(int i, byte b)
        {
            char[] letters  = payload.ToCharArray();
            letters[i]      = (char)b;
            payload         = string.Join("", letters);
        }

        internal void addByte(byte b)
        {
            payload = string.Join(payload,(char)b);
        }

        internal void clear()
        {
            payload = "";
        }

        internal int getCount()
        {
            return payload.Length;
        }

        internal bool isFull()
        {
            return getCount() >= LEN_MAX_PAYLOAD;
        }

        internal bool isEmpty()
        {
            return getCount() <= 0;
        }

        internal string getPayLoad()
        {
            return payload;
        }

        internal void setPayLoad(string str)
        {
            payload = str;
        }
    }
}
