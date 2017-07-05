using System;
using System.Diagnostics;

namespace GoodsTracker
{
    public enum StatusRx
    {
        RX_FRAME_INIT,
        RX_FRAME_BEGIN,
        RX_FRAME_RX_START,
        RX_FRAME_RX_FRAME,
        RX_FRAME_RX_END,
        RX_FRAME_RX_NL,
        RX_FRAME_RX_CR,
        RX_FRAME_OK,
        RX_FRAME_NOK,
    };

    public class CONST_CHAR
    {        
        public const char RX_FRAME_START    = '[';
        public const char RX_FRAME_END      = ']';
        public const char CR                = '\r';
        public const char LF                = '\n';
        public const char SEPARATOR         = ':';
        public const char ASTERISCO         = '*';
    }

    class Protocol
    {
        IOCommunication     IOComunic;
        StatusRx            statusRx = StatusRx.RX_FRAME_INIT;
        DataFrame           rxFrame;
        Stopwatch stopTx    = new Stopwatch();
        static int count    = 0;

        public Protocol(IOCommunication io)
        {
            IOComunic = io;
        }

        public void doCommunication()
        {
            processTx();
            processRx();
        }

        private void processTx()
        {
            try
            {
                if (stopTx.Elapsed.Milliseconds > 100)
                {
                    if (Communication.isAnyQueueCmd())
                    {
                        Cmd cmd = Communication.getNextCmd();
                        cmd.Header.Count    = count++;

                        DataFrame frame     = new DataFrame(cmd.Header,cmd.Payload);

                        writeTx(frame);

                        Communication.removeCmd(cmd);
                        Communication.addTxCmd(cmd);

                        printTx(frame,"TX OK: ");

                        stopTx.Restart();
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Transmissao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        private void processRx()
        {
            try
            {
                switch (statusRx)
                {
                    default:
                    case StatusRx.RX_FRAME_INIT:        initRxCMD();        break;
                    case StatusRx.RX_FRAME_BEGIN:       rxStartCMD();       break;
                    case StatusRx.RX_FRAME_RX_START:    receiveFrame();     break;
                    case StatusRx.RX_FRAME_RX_FRAME:    receiveFrame();     break;
                    case StatusRx.RX_FRAME_RX_END:      rxCR();             break;
                    case StatusRx.RX_FRAME_RX_CR:       rxNL();             break;
                    case StatusRx.RX_FRAME_RX_NL:       verifyFrame();      break;
                    case StatusRx.RX_FRAME_OK:          acceptRxFrame();    break;
                    case StatusRx.RX_FRAME_NOK:         errorRxFrame();     break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Recepcao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        void rxStartCMD()
        {
            char ch;

            if (readByteRx(out ch))
            {
                if (ch == CONST_CHAR.RX_FRAME_START)
                {
                    clearRxFrame();

                    setStatusRx(StatusRx.RX_FRAME_RX_START);
                }
            }
        }

        void receiveFrame()
        {
            char ch;

            if (readByteRx(out ch))
            {
                if (ch == CONST_CHAR.RX_FRAME_START)
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
                else if (ch == CONST_CHAR.RX_FRAME_END)
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_END);
                }
                else
                {
                    rxFrame.putByte(ch);

                    setStatusRx(StatusRx.RX_FRAME_RX_FRAME);
                }
            }
        }

        void rxNL()
        {
            char ch;

            if (readByteRx(out ch))
            {
                if (ch == CONST_CHAR.LF)
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_NL);
                }
                else
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
            }
        }

        void rxCR()
        {
            char ch;

            if (readByteRx(out ch))
            {
                if (ch == CONST_CHAR.CR)
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_CR);
                }
                else
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
            }
        }

        void verifyFrame()
        {
            IDecoderFrame decoder = new DecoderFrame();
            AnsCmd ans;

            if (decoder.getValues(out ans, rxFrame))
            {
                Communication.acceptAnswer(ans);

                setStatusRx(StatusRx.RX_FRAME_OK);
            }
            else
            {
                setStatusRx(StatusRx.RX_FRAME_NOK);
            }
        }

        void acceptRxFrame()
        {
            printFrame(rxFrame,"RX OK: ");

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void errorRxFrame()
        {
            printFrame(rxFrame,"RX NO: ");

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void initRxCMD()
        {
            stopTx.Start();
            clearRxFrame();
            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void setStatusRx(StatusRx sts)
        {
            statusRx = sts;
        }

        void clearRxFrame()
        {
            rxFrame = new DataFrame();
        }

        internal bool readByteRx(out char ch)
        {
            return IOComunic.getRxData(out ch);
        }

        public void writeTx(DataFrame frame)
        {
            if (frame != null && !frame.isFrameEmpty())
            {
                char[] end = { CONST_CHAR.CR, CONST_CHAR.LF };

                IOComunic.putTxData(frame.ToCharArray());
                IOComunic.putTxData(end);
            }
        }

        public void publishAnswer(DataFrame frame)
        {
            if (frame != null && !frame.isFrameEmpty())
            {
                char[] end = { CONST_CHAR.CR, CONST_CHAR.LF };

                IOComunic.putRxData(frame.str().ToCharArray());
                IOComunic.putRxData(end);
            }
        }

        private void printFrame(DataFrame frame,string str)
        {
            Debug.WriteLine(str);
            foreach (char c in frame.Data)
                Debug.Write(c.ToString());
            Debug.Write("\r\n");
        }

        private void printTx(DataFrame frame, string str)
        {
            Debug.WriteLine("TX TO:{0}[{1}] {2}{3} ms", frame.Header.Resource, frame.Header.Count.ToString("D5"), stopTx.Elapsed.Seconds.ToString("D2"), stopTx.Elapsed.Milliseconds.ToString("D3"));
            printFrame(frame, str);
        }
    }
}