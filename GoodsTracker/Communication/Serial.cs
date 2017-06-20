using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class Serial
    {
        RingBuffer bufferRx, bufferTx;

        public Serial()
        {
            bufferRx = new RingBuffer(1024);
            bufferTx = new RingBuffer(1024);
        }

        internal bool putTxData(char data)
        {
            return bufferTx.putData(data);
        }

        internal bool putRxData(char data)
        {
            return bufferRx.putData(data);
        }

        internal bool getTxData(out char ch)
        {
            return bufferTx.getData(out ch);
        }

        internal bool getRxData(out char ch)
        {
            return bufferRx.getData(out ch);
        }

        internal bool hasTxData()
        {
            return bufferTx.hasData();
        }

        internal void clear()
        {
            bufferRx.initBuffer();

            bufferTx.initBuffer();
        }

        internal void putTxData(string str)
        {
            foreach (char c in str)
            {
                putTxData(c);
            }
        }

        internal void putRxData(string str)
        {
            foreach (char c in str)
            {
                putRxData(c);
            }
        }
    }
}
