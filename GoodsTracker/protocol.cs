using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    interface IDecoderFrameTx
    {
        void buildFrame(out TxFrame frame, Cmd cmd);
    }

    interface IDecoderFrameRx
    {
        bool FillCmd(out ParamCmd param, RxFrame frame);
    }

    enum StatusRx
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

    class Protocol
    {
        private static Protocol singleton = null;

        /* [LED01]\r\n*/
        const char CHAR_RX_FRAME_START  = '[';
        const char CHAR_RX_FRAME_END    = ']';
        const char CHAR_CR              = '\r';
        const char CHAR_LF			    = '\n';
        const char CHAR_SEPARATOR	    = ':';
        const char CHAR_NAK             = ((char)0x15);

        StatusRx    statusRx            = StatusRx.RX_FRAME_INIT;
        RingBuffer  bufferRx, bufferTx;
        RxFrame     rxFrame;

        List<Cmd>       queueCmd        = new List<Cmd>();
        List<ParamCmd>  queueParam      = new List<ParamCmd>();

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

        private Protocol()
        {
            bufferRx = new RingBuffer(256);
            bufferTx = new RingBuffer(256);
        }

        internal void process()
        {
            processTx();

            processRx();

            processQueue();
        }

        private void processQueue()
        {
            if (queueParam.Count > 0)
            {
                foreach (ParamCmd param in queueParam)
                {
                    foreach (Cmd cmd in queueCmd)
                    {
                        if (param.NameCmd == cmd.getName())
                        {
                            queueCmd.Remove(cmd);
                        }
                    }
                }
            }
        }

        public void processTx()
        {
            if (queueCmd.Count > 0)
            {
                IDecoderFrameTx decoder = new DecoderFrameTx();

                TxFrame frame = new TxFrame();

                decoder.buildFrame(out frame, queueCmd[0]);

                sendFrame(frame);
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

            if (getRxData(out ch))
            {
                if (ch == CHAR_RX_FRAME_START)
                {
                    clearRxFrame();

                    setStatusRx(StatusRx.RX_FRAME_RX_START);
                }
            }
        }

        void rxPayLoad()
        {
            byte ch;

            if (getRxData(out ch))
            {
                if (ch == CHAR_RX_FRAME_START || rxFrame.isFull())
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
                else if (ch == CHAR_RX_FRAME_END)
                {
                    if (rxFrame.isFull())
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

                    rxFrame.addByte(ch);
                }
            }
        }

        void rxNL()
        {
            byte ch;

            if (getRxData(out ch))
            {
                if (ch == CHAR_LF)
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

            if (getRxData(out ch))
            {

                if (ch == CHAR_CR)
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
            ParamCmd        param_cmd   = new ParamCmd();

            if (decoder.FillCmd(out param_cmd, rxFrame))
            {
                queueParam.Add(param_cmd);
            }

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void errorRxFrame()
        {
            /**/

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void sendFrame(TxFrame frame)
        {
            if (!frame.isEmpty())
            {
                string str = frame.getPayLoad();

                putTxData((byte)CHAR_RX_FRAME_START);

                foreach (char c in str)
                {
                    putTxData((byte)c);
                }

                putTxData((byte)CHAR_RX_FRAME_END);

                putTxData((byte)CHAR_CR);
                putTxData((byte)CHAR_LF);
            }
        }

        void initRxCMD()
        {
            clearRxFrame();

            bufferRx.initBuffer();

            bufferTx.initBuffer();

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void setStatusRx(StatusRx sts)
        {
            statusRx = sts;
        }

        void clearRxFrame()
        {
            rxFrame = new RxFrame();
        }

        bool putTxData(byte data)
        {
            return bufferTx.putData(data);
        }

        bool putRxData(byte data)
        {
            return bufferRx.putData(data);
        }

        bool getTxData(out byte ch)
        {
            return bufferTx.getData(out ch);
        }

        bool getRxData(out byte ch)
        {
            return bufferRx.getData(out ch);
        }

        bool hasTxData()
        {
            return bufferTx.hasData();
        }
    }
}
