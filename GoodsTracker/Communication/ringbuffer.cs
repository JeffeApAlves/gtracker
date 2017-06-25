using System;
using System.Text;

namespace GoodsTracker
{
    class RingBuffer
    {
        private object _lock = new object();
        const int DEFAULT_BUFFER_SIZE = 1024;

        char[] data  = new char[DEFAULT_BUFFER_SIZE];

        int index_producer;
        int index_consumer;
        int count;

        internal RingBuffer(int size)
        {
            if (size > 0)
            {
                data = new char[size];
            }

            init();
        }

        internal bool putData(char ch)
        {
            bool flag = false;

            lock (_lock)
            {
                if (!isFull())
                {

                    data[index_producer++] = ch;
                    index_producer %= data.Length;
                    count++;

                    flag = true;
                }
            }

            return flag;
        }

        internal bool getData(out char ch)
        {
            bool flag = false;

            ch = new char();

            lock (_lock)
            {
                if (hasData())
                {

                    ch = data[index_consumer++];
                    index_consumer %= data.Length;
                    count--;

                    flag = true;
                }
            }

            return flag;
        }

        internal int getCount()
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

        internal void init()
        {
            index_consumer  = 0;
            index_producer  = 0;
            count           = 0;

            for (uint i = 0; i < data.Length; i++)
            {
                data[i] = (char)0;
            }
        }
    }
}
