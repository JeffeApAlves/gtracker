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

    public struct CONST_CHAR
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
        public acceptAnswerHandler acceptAnswer { get; set; }

        IOCommunication IOComunic;
        StatusRx            statusRx = StatusRx.RX_FRAME_INIT;
        DataFrame           rxFrame;

        public Protocol(IOCommunication io)
        {
            IOComunic = io;
        }

        public void receive()
        {
            while (IOComunic.hasAnyData())
            {
                switch (statusRx)
                {
                    default:
                    case StatusRx.RX_FRAME_INIT: initRxCMD(); break;
                    case StatusRx.RX_FRAME_BEGIN: rxStartCMD(); break;
                    case StatusRx.RX_FRAME_RX_START: receiveFrame(); break;
                    case StatusRx.RX_FRAME_RX_FRAME: receiveFrame(); break;
                    case StatusRx.RX_FRAME_RX_END: rxCR(); break;
                    case StatusRx.RX_FRAME_RX_CR: rxNL(); break;
                    case StatusRx.RX_FRAME_RX_NL: verifyFrame(); break;
                    case StatusRx.RX_FRAME_NOK: errorRxFrame(); break;
                }
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
            FrameSerialization decoder = new DecoderFrame();
            AnsCmd ans;

            if (decoder.decode(out ans, rxFrame))
            {
                if (ans != null)
                {
                    acceptAnswer?.Invoke(ans);

                    setStatusRx(StatusRx.RX_FRAME_OK);
                }

                // Fazer algo ????

                setStatusRx(StatusRx.RX_FRAME_BEGIN);
            }
            else
            {
                setStatusRx(StatusRx.RX_FRAME_NOK);
            }
        }

        void errorRxFrame()
        {
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

        internal bool readByteRx(out char ch)
        {
            return IOComunic.getRxData(out ch);
        }

        public bool writeTx(DataFrame frame)
        {
            bool flag = false;

            if (frame != null && !frame.isFrameEmpty())
            {
                char[] end = { CONST_CHAR.CR, CONST_CHAR.LF };

                IOComunic.putTxData(frame.ToCharArray());
                IOComunic.putTxData(end);
                
                flag = true;
            }

            return flag;
        }

        public void sendFrame(DataFrame frame)
        {
            if (frame != null && !frame.isFrameEmpty())
            {
                char[] end = { CONST_CHAR.CR, CONST_CHAR.LF };

                IOComunic.putRxData(frame.str().ToCharArray());
                IOComunic.putRxData(end);
            }
        }
    }
}