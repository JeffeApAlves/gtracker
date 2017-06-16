using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class RingBuffer
    {
        const int DEFAULT_BUFFER_SIZE = 64;

        byte[] data  = null;

        int index_producer;
        int index_consumer;
        uint count;

        internal RingBuffer(int size)
        {
            data = new byte[size>0?size: DEFAULT_BUFFER_SIZE];
        }

        internal bool putData(byte ch)
        {
            bool flag = false;

            if (!isFull())
            {

                data[index_producer++] = ch;
                index_producer %= data.Length;
                count++;

                flag = true;
            }

            return flag;
        }

        internal bool getData(out byte ch)
        {
            bool flag = false;

            ch = new byte();

            if (hasData())
            {

                ch = data[index_consumer++];
                index_consumer %= data.Length;
                count--;

                flag = true;
            }

            return flag;
        }

        internal uint getCount()
        {
            return count;
        }

        internal bool isFull()
        {
            return getCount() >= data.Length;
        }

        internal bool hasData()
        {
            return getCount() > 0;
        }

        internal void initBuffer()
        {
            index_consumer  = 0;
            index_producer  = 0;
            count           = 0;

            for (uint i = 0; i < data.Length; i++)
            {
                data[i] = 0;
            }
        }

    }
}
