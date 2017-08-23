using System;
using RabbitMQ.Client;
using System.Text;
using System.Diagnostics;
using RabbitMQ.Client.Events;

namespace GoodsTracker
{
    class RABBITMQ{

        public const string EXCHANGE_ANS    = "ANS";
        public const string EXCHANGE_CMD    = "CMD";
        public const string HOSTNAME        = "localhost";
        public const int PORT               = 5672;
        public const string PW              = "senai";
        public const string USER            = "senai";
        public const string VHOST           = "/";
        public const string CMD_QUEUE       = "CMD";
        public const string TLM_QUEUE       = "TLM";
    }

    class RabbitMQ
    {
        public acceptAnswerHandler acceptAnswer { get; set; }
        ConnectionFactory       factory;
        private IConnection     connection;
        private IModel          channel;
        EventingBasicConsumer   consumer;
   
        public bool Close()
        {
            channel.Close();
            return true;
        }

        public bool Open()
        {
            bool flg = false;

            try
            {
                factory = new ConnectionFactory()
                {
                    HostName = RABBITMQ.HOSTNAME,
                    UserName = RABBITMQ.USER,
                    Password = RABBITMQ.PW,
                    VirtualHost = RABBITMQ.VHOST,
                    Port = RABBITMQ.PORT,
                };

                connection = factory.CreateConnection();
                channel = connection.CreateModel();

                createExchanges();
                createQueues();
                createBinds();
                createConsumers();
            }
            catch (Exception e)
            {
                Console.WriteLine("Problema na conexao com o broker de mensagem RabbitMQ");
                Console.WriteLine(e.Message);
            }
  
            return flg;
        }

        private void createConsumers()
        {
            consumer = new EventingBasicConsumer(channel);

            consumer.Received += (model, ea) =>
            {
                var body                = ea.Body;
                var message             = Encoding.UTF8.GetString(body);
                var routingKey          = ea.RoutingKey;

                IDecoderFrame decoder   = new DecoderFrame();
                AnsCmd ans;
                DataFrame frame         = new DataFrame();
                frame.Data              = message + ":";
                frame.Data              = frame.Data+frame.checkSum().ToString("X2");

                if (decoder.getValues(out ans, frame))
                {
                    acceptAnswer?.Invoke(ans);
                }
            };

            BaseCommunication[] list = Communication.getArrayOfUnit();

            foreach (BaseCommunication c in list){
                channel.BasicConsume(queue:     RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                     autoAck:     true,
                                     consumer:  consumer);
            }
        }

        private void createExchanges()
        {
            channel.ExchangeDeclare(exchange: RABBITMQ.EXCHANGE_CMD, type: "direct");
            channel.ExchangeDeclare(exchange: RABBITMQ.EXCHANGE_ANS, type: "direct");
        }

        private void createQueues()
        {
            BaseCommunication[] list = Communication.getArrayOfUnit();

            foreach (BaseCommunication c in list)
            {
                channel.QueueDeclare(   queue: RABBITMQ.CMD_QUEUE + c.Address.ToString("D5"), 
                                        durable: false,
                                        exclusive: false, 
                                        autoDelete: false, 
                                        arguments: null);

                channel.QueueDeclare(   queue: RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                        durable: false,
                                        exclusive: false,
                                        autoDelete: false,
                                        arguments: null);

            }
        }

        private void createBinds()
        {
            BaseCommunication[] list = Communication.getArrayOfUnit();

            foreach (BaseCommunication c in list)
            {
                channel.QueueBind(  queue:      RABBITMQ.CMD_QUEUE + c.Address.ToString("D5"), 
                                    exchange:   RABBITMQ.EXCHANGE_CMD, 
                                    routingKey: c.Address.ToString("D5"));

                channel.QueueBind(  queue:      RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                    exchange:   RABBITMQ.EXCHANGE_ANS,
                                    routingKey: c.Address.ToString("D5"));
            }
        }

        private void processRx()
        {
            try
            {
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Recepcao AMQP");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        public bool publishCMD(DataFrame frame)
        {
            bool flag = true;

            try {

                var body = Encoding.UTF8.GetBytes(frame.Data);

                channel.BasicPublish(exchange: RABBITMQ.EXCHANGE_CMD,
                                        routingKey: frame.Header.Dest.ToString("D5"),
                                        basicProperties: null,
                                        body: body);
            }catch(Exception e){

                Console.WriteLine(e);
                flag = false;
            }

            return flag;
        }

        public void publishFrame(DataFrame frame)
        {
            var body = Encoding.UTF8.GetBytes(frame.Data);

            channel.BasicPublish(exchange: RABBITMQ.EXCHANGE_ANS,
                                    routingKey: frame.Header.Address.ToString("D5"),
                                    basicProperties: null,
                                    body: body);

        }
    }
}
