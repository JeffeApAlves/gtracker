using System.IO.Ports;
using System.Threading;

namespace GoodsTracker
{
    class Serial
    {
        static AutoResetEvent autoEvent;

        static private object _lock = new object();
        static SerialPort port=null;
        static RingBuffer bufferRx=new RingBuffer(8*1024);


        internal static bool putRxData(char data)
        {
            return bufferRx.putData(data);
        }

        internal static bool getRxData(out char ch)
        {
             bool flg = false;

            autoEvent.WaitOne();

//            lock (_lock)
            //{

                flg = bufferRx.getData(out ch);
            //}


            return flg;
        }

        internal static void clearBuffer()
        {
            bufferRx.initBuffer();
        }

        internal static void putTxData(char[] str)
        {
            if (port != null && port.IsOpen)
            {
                port.Write(str, 0, str.Length);
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

                autoEvent = new AutoResetEvent(false);

                port = new SerialPort("COM3",57600, Parity.None, 8, StopBits.One);

                port.ReadBufferSize     = 128;
                port.WriteBufferSize    = 128;
                port.Handshake = Handshake.None;
                port.DataReceived += new SerialDataReceivedEventHandler(_serialPort_DataReceived);

                port.ReadTimeout    = 50;
                port.WriteTimeout   = 50;

                port.Open();

                port.DiscardInBuffer();
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
            autoEvent.Reset();

            lock (_lock)
            {

                byte[] buffer = new byte[64];

                int bytesRead = port.Read(buffer, 0, 64);

                if (bytesRead > 0)
                {
                    for (int i = 0; i < bytesRead; i++)
                    {
                        putRxData((char)buffer[i]);
                    }
                }
            }

            autoEvent.Set();
        }

        public static void Close()
        {
            port.Close();
        }
    }
}
