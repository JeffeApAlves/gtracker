using System;
using System.Text;

namespace GoodsTracker
{
    class RingBuffer
    {
        const int DEFAULT_BUFFER_SIZE = 1024;

        char[] data  = null;

        int index_producer;
        int index_consumer;
        int count;

        internal RingBuffer(int size)
        {
            data = new char[size>0?size: DEFAULT_BUFFER_SIZE];
        }

        internal bool putData(char ch)
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

        internal bool getData(out char ch)
        {
            bool flag = false;

            ch = new char();

            if (hasData())
            {

                ch = data[index_consumer++];
                index_consumer %= data.Length;
                count--;

                flag = true;
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

        internal void initBuffer()
        {
            index_consumer  = 0;
            index_producer  = 0;
            count           = 0;

            for (uint i = 0; i < data.Length; i++)
            {
                data[i] = (char)0;
            }
        }
/*
        internal void putData(string dados)
        {
            //            data = Encoding.ASCII.GetBytes(dados);


            /*
                        string pattern      = @",";
                        string replacement  = "";

                        Regex rgx = new Regex(pattern);
                        string result = rgx.Replace(dados, replacement);
            
            char[] source = dados.ToCharArray();

            if(source!=null && source.Length > 0)
            {
                for(int i = 0; i < source.Length; i++)
                {
                    putData(source[i]);
                }
//                Array.Copy(source, data, dados.Length);
//                count = dados.Length;
            }
        }*/
    }
}
