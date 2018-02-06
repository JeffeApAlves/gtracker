namespace GoodsTracker
{
    class SerialCommunication : Communication
    {
        static Protocol channel = null;
        static Serial com_port  = null;

        public override void Init()
        {
            com_port = new Serial();
            com_port.Open();

            if (channel == null)
            {
                channel = new Protocol(com_port);
            }

            channel.acceptAnswer = acceptAnswer;

            start();
        }

        public override void DeInit()
        {
            com_port.Close();
        }

        public override bool sendCmd(Cmd cmd)
        {
            CommunicationFrame frame = new FrameSerial(cmd.Header, cmd.Payload);
            channel.writeTx(frame);
            printTxFrame("TX", frame);

            return true;
        }

        public override bool sendAns(AnsCmd ans)
        {
            bool        flag_res = false;
            CommunicationFrame   frame = new FrameSerial();

            frame.encode(ans);

            if (channel.writeTx(frame))
            {
                flag_res = true;
                printTxFrame("TX", frame);
            }

            return flag_res ;
        }

        public override bool receive()
        {
            channel.receive();
 
            return true;
        }

        /*
         * Callback chamada quando um frame é recebido com sucesso
         * 
         **/
        acceptAnswerHandler acceptAnswer = (AnsCmd ans) =>
        {
            Cmd cmd = searchCmdOfAnswer(ans);

            addAns(ans);
            acceptAnswerDevice(cmd,ans);
            removeTxCmd(cmd);
            removeAns(ans);
        };
    }
}
