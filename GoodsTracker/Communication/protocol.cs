using System;

namespace GoodsTracker
{
    public enum StatusRx
    {
        RX_FRAME_INIT,
        RX_FRAME_BEGIN,
        RX_FRAME_RX_START,
        RX_FRAME_RX_PAYLOAD,
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
        public const char NAK               = ((char)0x15);
        public const char ASTERISCO         = '*';
    }

    class Protocol : ThreadRun
    {
        static Protocol singleton = null;

        StatusRx            statusRx        = StatusRx.RX_FRAME_INIT;
        CommunicationFrame  rxFrame;
        CommunicationUnit   currentUnit;

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
            currentUnit = CommunicationUnit.getNextUnit();

            if (currentUnit != null)
            {
                processTx();
                processRx();
                currentUnit.processQueue();
            }
        }

        public void processTx()
        {
            if (CommunicationUnit.isAnyCmd())
            {
                IDecoderFrameTx decoder     = new DecoderFrame();
                CommunicationFrame frame    = new CommunicationFrame();

                if(decoder.setHeader(ref frame, currentUnit.getNextCmd()))
                {
                    sendFrame(frame);
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
                case StatusRx.RX_FRAME_RX_START:    rxPayLoad();        break;
                case StatusRx.RX_FRAME_RX_PAYLOAD:  rxPayLoad();        break;
                case StatusRx.RX_FRAME_RX_END:      rxCR();             break;
                case StatusRx.RX_FRAME_RX_CR:       rxNL();             break;
                case StatusRx.RX_FRAME_RX_NL:       verifyCheckSum();   break;
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

        void rxPayLoad()
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
                    rxFrame.addByteInFrame(ch);

                    setStatusRx(StatusRx.RX_FRAME_RX_PAYLOAD);
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

        void verifyCheckSum()
        {
            int checksum = rxFrame.checkSum();

            setStatusRx(StatusRx.RX_FRAME_OK);
        }

        void acceptRxFrame()
        {
            IDecoderFrameRx decoder     = new DecoderFrame();
            AnsCmd          ans;

            if (decoder.getValues(out ans, rxFrame))
            {
                CommunicationUnit.addAns(ans);
            }

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void errorRxFrame()
        {
            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void initRxCMD()
        {
            clearRxFrame();
            Serial.clearBuffer();
            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void setStatusRx(StatusRx sts)
        {
            statusRx = sts;
        }

        void clearRxFrame()
        {
            rxFrame = new CommunicationFrame();
        }

        internal void init()
        {
            setTime(50);

            Serial.Open();
        }

        void sendFrame(CommunicationFrame frame)
        {
            if (frame != null && !frame.isPayLoadEmpty())
            {
                Serial.putTxData(frame.strOfFrame());
                Serial.putTxData(CONST_CHAR.CR);
                Serial.putTxData(CONST_CHAR.LF);

                /*Serial.putTxData(CONST_CHAR.RX_FRAME_START);
                Serial.putTxData(frame.Frame);
                Serial.putTxData(CONST_CHAR.SEPARATOR);
                Serial.putTxData(frame.checkSum().ToString());
                Serial.putTxData(CONST_CHAR.RX_FRAME_END);
                Serial.putTxData(CONST_CHAR.CR);
                Serial.putTxData(CONST_CHAR.LF);*/
            }
        }

        internal void setFrameRx(CommunicationFrame frame)
        {
            if (frame != null)
            {
                Serial.putRxData(frame.strOfFrame());
                Serial.putRxData(CONST_CHAR.CR);
                Serial.putRxData(CONST_CHAR.LF);
            }
        }
    }
}
