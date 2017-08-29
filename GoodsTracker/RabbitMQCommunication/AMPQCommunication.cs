namespace GoodsTracker 
{
    /*
     * Comunicação via broker de mensagem
     * 
     */
    class AMPQCommunication : Communication
    {
        static RabbitMQ amqp = null;

        public override void Init()
        {
            amqp = new RabbitMQ();
            amqp.acceptAnswer = acceptAnswer;
            amqp.Open();
            start();
        }

        public override void DeInit()
        {
            amqp.Close();
        }

        public override bool sendCmd(Cmd cmd)
        {
            bool flag = false;

            CommunicationFrame frame = new FrameAMQP(cmd.Header, cmd.Payload);

            if (amqp.publishCMD(frame))
            {
                printTxFrame("AMPQP", frame);
                flag = true;
            }
            
            return flag;
        }

        public override void register(DeviceBase device)
        {
            base.register(device);
            amqp.register(device);
        }

        public override bool sendAns(AnsCmd ans)
        {
            bool flag_res = false;
            CommunicationFrame frame = new FrameAMQP();

            frame.encode(ans);

            if (amqp.publishAns(frame))
            {
                flag_res = true;
                printTxFrame("AMPQ", frame);
            }

            return flag_res;
        }

        /**
         * 
         * Processsa a pilha de recebimento do protocolo
         * No caso do RabbitMQ não se faz necessario pois a API irá chamar a callback previamente associada
         * 
         */
        public override bool receive()
        {
            return true;
        }

        /*
         * Callback chamada quando um frame é recebido com sucesso
         * 
         */
        acceptAnswerHandler acceptAnswer = (AnsCmd ans) =>
        {
            addAns(ans);
            acceptAnswerDevice(null,ans);
            removeAns(ans);
        };
    }
}
