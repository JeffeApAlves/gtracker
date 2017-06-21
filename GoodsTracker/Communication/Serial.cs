using System.IO.Ports;

namespace GoodsTracker
{
    class Serial
    {
        static SerialPort port=null;
        static RingBuffer bufferRx=new RingBuffer(1024);
//        static RingBuffer bufferTx=new RingBuffer(1024);

        internal static bool putTxData(char data)
        {
            port.Write(data.ToString());
            return true;
            //return bufferTx.putData(data);
        }

        internal static bool putRxData(char data)
        {
            return bufferRx.putData(data);
        }

        internal static bool getRxData(out char ch)
        {
            return bufferRx.getData(out ch);
        }
/*
        internal static bool hasTxData()
        {
            return bufferTx.hasData();
        }*/

        internal static void clearBuffer()
        {
            bufferRx.initBuffer();
//            bufferTx.initBuffer();
        }

        internal static void putTxData(string str)
        {
            foreach (char c in str)
            {
                putTxData(c);
            }
        }

        internal static void putRxData(string str)
        {
            foreach (char c in str)
            {
                putRxData(c);
            }
        }

        public static bool Open()
        {
            try
            {
                clearBuffer();
                
                port = new SerialPort("COM1",9600, Parity.None, 8, StopBits.One);

                port.ReadBufferSize     = 1024;
                port.WriteBufferSize    = 1024;
                port.Handshake = Handshake.None;
                port.DataReceived += new SerialDataReceivedEventHandler(_serialPort_DataReceived);

                port.ReadTimeout    = 50;
                port.WriteTimeout   = 50;

                port.Open();
            }
            catch
            {
                return false;
            }

            return true;
        }

        /*
         * 
         * Evento recepcao
         * 
         */
        private static void _serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] buffer = new byte[port.ReadBufferSize];

            int bytesRead = port.Read(buffer, 0, buffer.Length);

            for(int i=0;i< bytesRead; i++)
            {
                putRxData((char)buffer[i]);
            }
        }

    }
}
