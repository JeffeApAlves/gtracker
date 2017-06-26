using System;
using System.Diagnostics;
using System.Text;
using System.Threading;

namespace GoodsTracker
{
    class RingBuffer
    {
        private object _lock = new object();
        const int DEFAULT_BUFFER_SIZE = 1024;
        private static Semaphore semaforo;
        Stopwatch stopPUT = new Stopwatch();
        Stopwatch stopGET = new Stopwatch();

        char[] data  = new char[DEFAULT_BUFFER_SIZE];

        int index_producer;
        int index_consumer;
        int count;

        public int Count { get => count; set => count = value; }

        internal RingBuffer(int size)
        {
            semaforo = new Semaphore(1, 1);

            if (size > 0)
            {
                data = new char[size];
            }

            init();
        }

        internal bool putData(char ch)
        {
            bool flag = false;

            if (!isFull())
            {
                semaforo.WaitOne();

                data[index_producer++] = ch;
                index_producer %= data.Length;
                count++;

                semaforo.Release();

                Console.WriteLine("PUT: {0} '{1}' P/C:{2}/{3}-Count:{4}", stopPUT.Elapsed.Milliseconds.ToString("D5"), ch, index_producer.ToString("D5"), index_consumer.ToString("D5"), count.ToString("D5"));
                stopPUT.Start();

                flag = true;
            }
            else
            {
//                Console.WriteLine("RingBuffer FULL {0}",count);
            }

            return flag;
        }

        internal bool getData(out char ch)
        {
            bool flag = false;

            ch = new char();

            if (hasData())
            {
                semaforo.WaitOne();

                ch = data[index_consumer++];
                index_consumer %= data.Length;
                count--;

                semaforo.Release();

                Console.WriteLine("GET: {0} '{1}' P/C:{2}/{3}-Count:{4}", stopGET.Elapsed.Milliseconds.ToString("D5"), ch, index_producer.ToString("D5"), index_consumer.ToString("D5"), count.ToString("D5"));
                stopGET.Start();

                flag = true;
            }
            else
            {
//                Console.WriteLine("RingBuffer EMPTY {0}",count);
            }


            return flag;
        }

        internal bool isFull()
        {
            return count >= data.Length;
        }

        internal bool hasData()
        {
            return count > 0;
        }

        internal void init()
        {
            index_consumer  = 0;
            index_producer  = 0;
            count           = 0;

            semaforo.WaitOne();

            for (uint i = 0; i < data.Length; i++)
            {
                data[i] = (char)0;
            }

            semaforo.Release();
        }
    }
}
