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
            bufferRx = new RingBuffer(256);
            bufferTx = new RingBuffer(256);
        }

        internal bool putTxData(byte data)
        {
            return bufferTx.putData(data);
        }

        internal bool putRxData(byte data)
        {
            return bufferRx.putData(data);
        }

        internal bool getTxData(out byte ch)
        {
            return bufferTx.getData(out ch);
        }

        internal bool getRxData(out byte ch)
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

        internal void putTxData(char data)
        {
            putTxData((byte)data);
        }

        internal void putTxData(string str)
        {
            foreach (char c in str)
            {
                putTxData(c);
            }
        }
    }
}
