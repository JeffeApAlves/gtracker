using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class RingBuffer
    {
        const uint BUFFER_SIZE = 64;

        byte[] data  = new byte[BUFFER_SIZE];

        uint index_producer;
        uint index_consumer;
        uint count;

        public bool putData(byte ch)
        {
            bool flag = false;

//            EnterCritical();

            if (!isFull())
            {

                data[index_producer++] = ch;
                index_producer %= BUFFER_SIZE;
                count++;

                flag = true;
            }

//            ExitCritical();

            return flag;
        }
        //------------------------------------------------------------------------

        public bool getData(out byte ch)
        {
            bool flag = false;

            ch = new byte();

//            EnterCritical();

            if (hasData())
            {

                ch = data[index_consumer++];
                index_consumer %= BUFFER_SIZE;
                count--;

                flag = true;
            }

//            ExitCritical();

            return flag;
        }
        //------------------------------------------------------------------------

        public uint getCount()
        {
            return count;
        }
        //------------------------------------------------------------------------

        public bool isFull()
        {
            return getCount() >= BUFFER_SIZE;
        }
        //------------------------------------------------------------------------

        public bool hasData()
        {
            return getCount() > 0;
        }
        //------------------------------------------------------------------------

        public void initBuffer()
        {
            index_consumer = 0;
            index_producer = 0;
            count = 0;

            for (uint i = 0; i < BUFFER_SIZE; i++)
            {

                data[i] = 0;
            }
        }
        //------------------------------------------------------------------------
    }
}
