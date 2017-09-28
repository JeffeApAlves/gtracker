using System;
using RabbitMQ.Client;
using System.Text;
using RabbitMQ.Client.Events;
using System.Diagnostics;

namespace GoodsTracker
{
    struct RABBITMQ{

        public const string HOSTNAME        = "192.168.42.1";
        public const int PORT               = 5672;
        public const string PW              = "senai_pc";
        public const string USER            = "senai_pc";
        public const string VHOST           = "/";

        public const string LOG_EXCHANGE    = "log";
        public const string ANS_EXCHANGE    = "ans";
        public const string INFO_EXCHANGE   = "info";
        public const string ERROR_EXCHANGE  = "erro";
        public const string CMD_EXCHANGE    = "cmd";

        public const string CMD_QUEUE       = "CMD";
        public const string TLM_QUEUE       = "TLM";
        public const string LOG_QUEUE       = "LOG";
        public const string INFO_QUEUE      = "INFO";
        public const string ERROR_QUEUE     = "ERRO";
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
                    HostName    = RABBITMQ.HOSTNAME,
                    UserName    = RABBITMQ.USER,
                    Password    = RABBITMQ.PW,
                    VirtualHost = RABBITMQ.VHOST,
                    Port        = RABBITMQ.PORT,
                };

                connection  = factory.CreateConnection();
                channel     = connection.CreateModel();

                if (channel != null)
                {
                    createExchanges();
                    createQueues();
                    createBinds();
                    createConsumers();
                    registerAll();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine("Problema na conexao com o broker de mensagem RabbitMQ");
                Debug.WriteLine(e.Message);
            }
  
            return flg;
        }

        private void createConsumers()
        {
            consumer = new EventingBasicConsumer(channel);

            consumer.Received += (model, ea) =>
            {
                var body                    = ea.Body;
                var message                 = Encoding.UTF8.GetString(body);
                var routingKey              = ea.RoutingKey;

                FrameSerialization serialization  = Communication.createSerialization();
                AnsCmd ans;
                CommunicationFrame frame         = new FrameAMQP();
                frame.Data              = message + ":";
                frame.Data              = frame.Data+frame.checkSum().ToString("X2");

                if (serialization.decode(out ans, frame))
                {
                    acceptAnswer?.Invoke(ans);
                }
            };
        }

        private void createExchanges()
        {
            channel.ExchangeDeclare(exchange: RABBITMQ.CMD_EXCHANGE,    type: "direct");
            channel.ExchangeDeclare(exchange: RABBITMQ.ANS_EXCHANGE,    type: "direct");
            channel.ExchangeDeclare(exchange: RABBITMQ.LOG_EXCHANGE,    type: "direct");
            channel.ExchangeDeclare(exchange: RABBITMQ.INFO_EXCHANGE,   type: "direct");
            channel.ExchangeDeclare(exchange: RABBITMQ.ERROR_EXCHANGE,  type: "direct");
        }

        private void createQueues()
        {
            channel.QueueDeclare(queue: RABBITMQ.LOG_QUEUE,
                           durable: false,
                           exclusive: false,
                           autoDelete: false,
                           arguments: null);

            channel.QueueDeclare(queue: RABBITMQ.INFO_QUEUE,
                           durable: false,
                           exclusive: false,
                           autoDelete: false,
                           arguments: null);

            channel.QueueDeclare(queue: RABBITMQ.ERROR_QUEUE,
                          durable: false,
                          exclusive: false,
                          autoDelete: false,
                          arguments: null);

        }

        private void createBinds()
        {
           channel.QueueBind(   queue:      RABBITMQ.LOG_QUEUE,
                                exchange:   RABBITMQ.LOG_EXCHANGE,
                                routingKey: "");

            channel.QueueBind(  queue:      RABBITMQ.INFO_QUEUE,
                                exchange:   RABBITMQ.INFO_EXCHANGE,
                                routingKey: "");

            channel.QueueBind(  queue:      RABBITMQ.ERROR_QUEUE,
                                exchange:   RABBITMQ.ERROR_EXCHANGE,
                                routingKey: "");
        }

        public bool publishCMD(CommunicationFrame frame)
        {
            return publishFrame(exchange:   RABBITMQ.CMD_EXCHANGE,
                                route:      "cmd." + frame.Header.Dest.ToString("D5"),
                                frame:      frame);
        }

        public bool publishAns(CommunicationFrame frame)
        {
            return publishFrame(exchange:   RABBITMQ.ANS_EXCHANGE,
                                route:      "ans." + frame.Header.Address.ToString("D5"),
                                frame:      frame);
        }

        public bool publishFrame(string exchange,string route,CommunicationFrame frame)
        {
            bool flag = true;

            try
            {
                var body = Encoding.UTF8.GetBytes(frame.Data);

                channel.BasicPublish(   exchange:           exchange,
                                        routingKey:         route,
                                        basicProperties:    null,
                                        body:               body);
            }
            catch (Exception e)
            {
                Debug.WriteLine("Erro ao publicar o frame no server");
                Debug.WriteLine(e);
                flag = false;
            }

            return flag;
        }

        public void registerAll()
        {
            DeviceBase[] list = Communication.getArrayOfDevices();

            foreach (DeviceBase c in list)
            {
                register(c);
            }
        }

        /*
         * 
         * Cria filas,bind relacionado a um dispositivo
         * 
         */
        public void register(DeviceBase c)
        {
            channel.QueueDeclare(    queue:      RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                     durable:    true,
                                     exclusive:  false,
                                     autoDelete: false,
                                     arguments:  null);

            channel.QueueDeclare(   queue:      RABBITMQ.CMD_QUEUE + c.Address.ToString("D5"),
                                    durable:    true,
                                    exclusive:  false,
                                    autoDelete: false,
                                    arguments:  null);

            channel.QueueBind(  queue:      RABBITMQ.CMD_QUEUE + c.Address.ToString("D5"),
                                exchange:   RABBITMQ.CMD_EXCHANGE,
                                routingKey: "cmd." + c.Address.ToString("D5"));

            channel.QueueBind(  queue:      RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                exchange:   RABBITMQ.ANS_EXCHANGE,
                                routingKey: "ans." + c.Address.ToString("D5"));

            channel.BasicConsume(queue: RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                      autoAck: true,
                      consumer: consumer);
        }
    }
}
