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

                //                LogConsole("PUT:", ch);

                semaforo.Release();

                flag = true;
            }
            else
            {
                //TODO
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

                LogConsole("GET:", ch);

                semaforo.Release();

                flag = true;
            }
            else
            {
                //TODO 
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

        void LogConsole(string str,char ch)
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(str);
            sb.Append(" ");
            sb.Append(stopPUT.Elapsed.Milliseconds.ToString("D5"));
            sb.Append(" ");
            sb.Append(count.ToString("D5"));
            sb.Append(" ");
            sb.Append(index_producer.ToString("D5"));
            sb.Append("\\");
            sb.Append(index_consumer.ToString("D5"));
            sb.Append(" ");
            sb.Append(ch);

            Console.WriteLine(sb);
            stopPUT.Start();
        }
    }
}
