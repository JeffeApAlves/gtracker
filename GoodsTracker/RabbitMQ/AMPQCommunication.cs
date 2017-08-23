using System;

namespace GoodsTracker 
{
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
            try
            {
                DataFrame frame = new DataFrame(cmd.Header, cmd.Payload);
                amqp.publishCMD(frame);
                printTx("PB",frame);

                flag = true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                flag = false;
            }

            return flag;
        }

        public override void register(DeviceBase unit)
        {
            amqp.register(unit);
        }

        public override bool sendAns(AnsCmd ans)
        {
            bool flag = false;

            try
            {
                DataFrame frame = null;

                if (ans.Header.Resource.Equals(RESOURCE.TLM))
                {
                    PayLoad payload;

                    DecoderFrame decoder = new DecoderFrame();
                    decoder.setValues(out payload, ans.Telemetria);

                    frame = new DataFrame(ans.Header, payload);
                }

                if (frame != null)
                {
                    amqp.publishAns(frame);
                    flag = true;
                    printTx("PB", frame);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                flag = false;
            }

            return flag;
        }

        /**
         * 
         * Processsa a pilha de recebimento do protocolo
         * 
         */
        public override bool receive()
        {
            // Nao fazer nada, pois a callback ira chamar direto a funcao delegate accepAnser previamente 
            // atribuida na propriedade
 
            return true;
        }

        /*
         * Callback chamada quando um frame é recebido com sucesso
         * 
         **/
        acceptAnswerHandler acceptAnswer = (AnsCmd ans) =>
        {
            addAns(ans);
            processAnswer(null,ans);
            removeAns(ans);
        };
    }
}
