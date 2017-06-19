using System.Collections.Generic;

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

    public enum Operation
    {
        RD,WR
    }

    public class CONST_CHAR
    {        
        public const char RX_FRAME_START    = '[';
        public const char RX_FRAME_END      = ']';
        public const char CR                = '\r';
        public const char LF                = '\n';
        public const char SEPARATOR         = ',';
        public const char NAK               = ((char)0x15);
        public const char ASTERISCO         = '*';
    }

    class Protocol : ThreadRun
    {
        static Protocol singleton = null;
        static List<CommunicationUnit> units = new List<CommunicationUnit>();

        StatusRx            statusRx        = StatusRx.RX_FRAME_INIT;
        Serial              serial;
        CommunicationFrame  rxFrame;
        CommunicationUnit   cur_unit;
        int                 index = 0;

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

        internal static List<CommunicationUnit> Units { get => units; set => units = value; }

        private Protocol()
        {
            serial = new Serial();
        }

        public override void run()
        {
            cur_unit = units[(index++)%units.Count];

            if (cur_unit != null)
            {
                processTx();

                processRx();

                cur_unit.processQueue();
            }
        }

        public void processTx()
        {
            if (CommunicationUnit.isAnyCmd())
            {
                IDecoderFrameTx decoder = new DecoderFrameTx();

                CommunicationFrame frame;

                if(decoder.setFrame(out frame, cur_unit))
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
            byte ch;

            if (serial.getRxData(out ch))
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
            byte ch;

            if (serial.getRxData(out ch))
            {
                if (ch == CONST_CHAR.RX_FRAME_START || rxFrame.isPayLoadEmpty())
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
                else if (ch == CONST_CHAR.RX_FRAME_END)
                {
                    if (rxFrame.isPayLoadEmpty())
                    {
                        setStatusRx(StatusRx.RX_FRAME_RX_END);
                    }
                    else
                    {
                        setStatusRx(StatusRx.RX_FRAME_NOK);
                    }
                }
                else
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_PAYLOAD);

                    rxFrame.addByteInFrame(ch);
                }
            }
        }

        void rxNL()
        {
            byte ch;

            if (serial.getRxData(out ch))
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
            byte ch;

            if (serial.getRxData(out ch))
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
            setStatusRx(StatusRx.RX_FRAME_OK);
        }

        void acceptRxFrame()
        {
            IDecoderFrameRx decoder     = new DecoderFrameRx();
            AnsCmd          ans         = new AnsCmd();
            ObjectValueRX   dadosRx;

            if (decoder.getValues(out dadosRx, rxFrame))
            {
                ans.DadosRx = dadosRx;

                CommunicationUnit.addAns(ans);
            }

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void errorRxFrame()
        {
            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void sendFrame(CommunicationFrame frame)
        {
            if (!frame.isPayLoadEmpty())
            {
                string payload = frame.PayLoad;

                serial.putTxData(CONST_CHAR.RX_FRAME_START);
                serial.putTxData(payload);
                serial.putTxData(CONST_CHAR.ASTERISCO);
                serial.putTxData(frame.checkSum().ToString());
                serial.putTxData(CONST_CHAR.RX_FRAME_END);
                serial.putTxData(CONST_CHAR.CR);
                serial.putTxData(CONST_CHAR.LF);
            }
        }

        void initRxCMD()
        {
            clearRxFrame();
            serial.clear();
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
    }
}
