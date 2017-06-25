using System.IO.Ports;
using System.Threading;

namespace GoodsTracker
{
    class Serial
    {
        const int SIZE_BUFFER_RX    = 512;
        const int SIZE_BUFFER_TX    = 512;
        const int SIZE_RING_BUFFER_RX = (1 * 1024);

        static AutoResetEvent autoEvent;

        static SerialPort port = null;
        static RingBuffer bufferRx = new RingBuffer(SIZE_RING_BUFFER_RX);

        internal static bool putRxData(char data)
        {
            return bufferRx.putData(data);
        }

        internal static bool getRxData(out char ch)
        {
             bool flg = false;

            autoEvent.WaitOne();

            flg = bufferRx.getData(out ch);

            return flg;
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
                bufferRx.init();

                autoEvent   = new AutoResetEvent(false);
                port        = new SerialPort("COM3",57600, Parity.None, 8, StopBits.One);

                port.ReadBufferSize     = SIZE_BUFFER_RX;
                port.WriteBufferSize    = SIZE_BUFFER_TX;
                port.Handshake          = Handshake.None;
                port.DataReceived += new SerialDataReceivedEventHandler(_serialPort_DataReceived);
                port.ReadTimeout        = 50;
                port.WriteTimeout       = 50;
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

            char[] buffer = new char[SIZE_BUFFER_RX];

            int bytesRead = port.Read(buffer, 0, SIZE_BUFFER_RX);

            if (bytesRead > 0)
            {
                for (int i = 0; i < bytesRead; i++)
                {
                    putRxData(buffer[i]);
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
