using System;

namespace GoodsTracker 
{
    class AMPQCommunication : Communication
    {
        static RabbitMQ amqp = null;

        public override void Init()
        {
            base.Init();

            amqp = new RabbitMQ();
            amqp.acceptAnswer = acceptAnswer;
            amqp.Open();
            start();
        }

        public override void DeInit()
        {
            base.DeInit();
            amqp.Close();
        }

        public override bool send(Cmd cmd)
        {
            bool flag = false;
            try
            {
                DataFrame frame = new DataFrame(cmd.Header, cmd.Payload);
                amqp.publishCMD(frame);
                printTx("TX",frame);

                flag = true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                flag = false;
            }

            return flag;
        }

        public override void send(DataFrame frame)
        {
            amqp.publishFrame(frame);
        }

        /**
         * 
         * Processsa a pilha de recebimento do protocolo
         * 
         */
        public override bool receive()
        {
            // Nao fazer nada pois a callback ira chamar direto a funcao delegate accepAnser previamente 
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
