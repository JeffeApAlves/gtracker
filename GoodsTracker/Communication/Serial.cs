using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;

namespace GoodsTracker
{
    class Serial
    {
        const int SIZE_BUFFER_RX    = 1024;
        const int SIZE_BUFFER_TX    = 1024;
        const int SIZE_RING_BUFFER_RX = (1 * 512);

        static SerialPort port = null;
        static RingBuffer bufferRx = new RingBuffer(SIZE_RING_BUFFER_RX);
        static Stopwatch stopSerial = new Stopwatch();
        /*
         * Cola na fila uma string
         */
        internal static bool putRxData(char data)
        {
            if (bufferRx.isFull())
            {
            }

            return bufferRx.putData(data);
        }

        /*
         * 
         * Le um caracter da fila Rx
         */ 
        internal static bool getRxData(out char ch)
        {
            bool flg = false;

            flg = bufferRx.getData(out ch);

            return flg;
        }

        /*
         * 
         * Coloca na fila Tx uma fila de string
         * 
         */
        internal static void putTxData(char[] str)
        {
            if (port != null && port.IsOpen)
            {
                port.Write(str, 0, str.Length);
            }           
        }

        /**
         * Coloca na fila Rx uma string 
         * 
         */ 
        internal static void putRxData(char[] str)
        {
            foreach (char c in str)
            {
                putRxData(c);
            }
        }

        /**
         * 
         * Abri a porta de comunicacao
         * 
         */ 
        public static bool Open()
        {
            try
            {
                bufferRx.init();

                port        = new SerialPort("COM3",57600, Parity.None, 8, StopBits.One);

                port.ReadBufferSize     = SIZE_BUFFER_RX;
                port.WriteBufferSize    = SIZE_BUFFER_TX;
                port.Handshake = Handshake.None;
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
            try
            {
                if (!bufferRx.isFull())
                {
                    char[] buffer = new char[SIZE_BUFFER_RX];
                    int bytesRead = port.Read(buffer, 0, SIZE_BUFFER_RX);

                    if (bytesRead > 0)
                    {
                        buffer[bytesRead] = (char)0;

                        //Console.WriteLine("COM: {0} '{1}'", stopSerial.Elapsed.Milliseconds.ToString("D5"), new string(buffer));

                        Console.Write("COM: {0} ", stopSerial.Elapsed.Milliseconds.ToString("D5"));

                        stopSerial.Start();

                        for (int i = 0; i < bytesRead; i++)
                        {
                            putRxData(buffer[i]);
                            Console.Write("{1}", buffer[i]);
                        }
                        Console.Write("\n");
                    }
                }
            }
            catch
            {
            }            
        }

        /**
         * 
         * Fecha a porta serial
         * 
         */
        public static void Close()
        {
            port.Close();
        }
    }
}
