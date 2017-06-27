using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Windows;

namespace GoodsTracker
{
    class Serial
    {
        const int SIZE_BUFFER_RX    = (2 * 1024);
        const int SIZE_BUFFER_TX    = (2 * 1024);
        const int SIZE_RING_BUFFER_RX = (8 * 1024);

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
                port.Handshake          = Handshake.None;
                port.ReadTimeout        = 50;
                port.WriteTimeout       = 50;
                port.DataReceived += new SerialDataReceivedEventHandler(_serialPort_DataReceived);
                port.ErrorReceived += new SerialErrorReceivedEventHandler(_serialPort_DataErroReceived);

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
            try
            {
                if (!bufferRx.isFull())
                {
                    SerialPort com = (SerialPort)sender;

                    if (com.BytesToRead > 0)
                    {
                        char[] buffer   = new char[com.BytesToRead];
                        int bytesRead   = com.Read(buffer, 0, buffer.Length);
                        int size        = bytesRead > buffer.Length ? buffer.Length : bytesRead;

                        if (size > 0)
                        {
                            StringBuilder sb = new StringBuilder();

                            for (int i = 0; i < size; i++)
                            {
                                putRxData(buffer[i]);

                                if (buffer[i] == '\n')
                                {
                                    sb.Append("\\n");

                                }
                                else if (buffer[i] == '\r')
                                {
                                    sb.Append("\\r");
                                }
                                else
                                {
                                    sb.Append(buffer[i]);
                                }

//                                LogConsole("COM:", buffer[i], bytesRead);
                            }

                            LogConsole("COM:", sb.ToString(), bytesRead);
                        }
                    }
                }
            }
            catch(System.IO.IOException ex)
            {
                Console.WriteLine("Erro no evento de recepcao dos dados");
                Console.WriteLine(ex.ToString());
                Debug.WriteLine(ex.ToString());
            }
        }

        private static void _serialPort_DataErroReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            MessageBox.Show(e.ToString());
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

        static void LogConsole(string str, string info,int num)
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(str);
            sb.Append(" ");
            sb.Append(stopSerial.Elapsed.Milliseconds.ToString("D5"));
            sb.Append(" ");
            sb.Append(num.ToString("D5"));
            sb.Append(" ");
            sb.Append("     ");
            sb.Append("\\");
            sb.Append("     ");
            sb.Append(" ");
            sb.Append(info);

            Console.WriteLine(sb.ToString());
            stopSerial.Start();
        }

        static void LogConsole(string str, char ch, int count)
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(str);
            sb.Append(" ");
            sb.Append(stopSerial.Elapsed.Milliseconds.ToString("D5"));
            sb.Append(" ");
            sb.Append(count.ToString("D5"));
            sb.Append(" ");
            sb.Append("     ");
            sb.Append("\\");
            sb.Append("     ");
            sb.Append(" ");
            sb.Append(ch);

            Console.WriteLine(sb.ToString());
            stopSerial.Start();
        }
    }
}
