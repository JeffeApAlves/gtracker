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

    public enum ResultExec
    {
        EXEC_UNSUCCESS = -3,
        INVALID_CMD = -2,
        INVALID_PARAM = -1,
        EXEC_SUCCESS = 0,
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

    class Protocol : ThreadRun
    {
        static Protocol singleton = null;

        StatusRx            statusRx = StatusRx.RX_FRAME_INIT;
        DataFrame           rxFrame;
        CommunicationUnit   currentUnit;
        private const int _TIME_COMMUNICATION    = 1;
        Stopwatch stopTx = new Stopwatch();
        static int count=0;

        //Singleton
        public static Protocol Communication
        {
            get
            {
                if (singleton == null)
                {
                    singleton = new Protocol();
                }

                return singleton;
            }
        }

        public override void run()
        {
            try
            {
                currentUnit = CommunicationUnit.getNextUnit();

                if (currentUnit != null)
                {
                    processTx();
                    processRx();
                    currentUnit.processQueues();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de comunicacao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        public void processTx()
        {
            if (stopTx.Elapsed.Milliseconds > 100)
            {
                if (CommunicationUnit.isAnyQueueCmd())
                {
                    DataFrame frame = new DataFrame();
                    Cmd cmd = currentUnit.getNextCmd();

                    cmd.Header.Count    = count++;
                    frame.Header        = cmd.Header;
                    frame.PayLoad       = cmd.Payload;

                        sendFrame(frame);

                        Debug.WriteLine("TX TO:{0}[{1}] {2}{3} ms", frame.Header.Resource, frame.Header.Count.ToString("D5"), stopTx.Elapsed.Seconds.ToString("D2"), stopTx.Elapsed.Milliseconds.ToString("D3"));

                        Debug.Write("TX OK: ");
                        foreach (char c in frame.Data)
                            Debug.Write(c.ToString());
                        Debug.Write("\r\n");

                        stopTx.Restart();
                    }
            }
        }

        void processRx()
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

        void rxStartCMD()
        {
            char ch;

            if (Serial.getRxData(out ch))
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

            if (Serial.getRxData(out ch))
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

            if (Serial.getRxData(out ch))
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

            if (Serial.getRxData(out ch))
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
                CommunicationUnit.addAns(ans);

                setStatusRx(StatusRx.RX_FRAME_OK);
            }
            else
            {
                setStatusRx(StatusRx.RX_FRAME_NOK);
            }
        }

        void acceptRxFrame()
        {
            // TODO ???
            Debug.Write("RX OK: ");

            foreach (char c in rxFrame.Data)
                Debug.Write(c.ToString());
            Debug.Write("\r\n");

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void errorRxFrame()
        {
            // TODO ???
            Debug.WriteLine("RX NO: ");

            foreach (char c in rxFrame.Data)
                Debug.Write(c.ToString());
            Debug.Write("\r\n");

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void initRxCMD()
        {
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

        internal void init()
        {
            stopTx.Start();
            setTime(_TIME_COMMUNICATION);
            Serial.Open();
        }

        void sendFrame(DataFrame frame)
        {
            if (frame != null && !frame.isFrameEmpty())
            {
                char[] end = { CONST_CHAR.CR, CONST_CHAR.LF };

                Serial.putTxData(frame.ToCharArray());
                Serial.putTxData(end);
            }
        }

        internal void setFrameRx(DataFrame frame)
        {
            if (frame != null)
            {
                Serial.putRxData(frame.str().ToCharArray());
                Serial.putRxData(CONST_CHAR.CR);
                Serial.putRxData(CONST_CHAR.LF);
            }
        }
    }
}
