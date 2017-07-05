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
        public const string PW              = "rna@1981";
        public const string USER            = "rna";
        public const string VHOST           = "/";
        public const string CMD_QUEUE       = "CMD";
        public const string TLM_QUEUE       = "TLM";
    }

    class RabbitMQ
    {
        ConnectionFactory       factory;
        private IConnection     connection;
        private IModel          channel;
        EventingBasicConsumer   consumer;
        Stopwatch               stopTx = new Stopwatch();
        static int              count = 0;

        public bool Close()
        {
            channel.Close();
            return true;
        }

        public bool Open()
        {
            bool flg = false;

            factory             = new ConnectionFactory();
            
            factory.HostName    = RABBITMQ.HOSTNAME;
            factory.UserName    = RABBITMQ.USER;
            factory.Password    = RABBITMQ.PW;
            factory.VirtualHost = RABBITMQ.VHOST;
            factory.Port        = RABBITMQ.PORT;

            connection          = factory.CreateConnection();
            channel             = connection.CreateModel();

            createExchanges();
            createQueues();
            createBinds();
            createConsumers();

            stopTx.Start();
            return flg;
        }

        public void doCommunication()
        {
            processTx();
            processRx();
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
                    Communication.acceptAnswer(ans);

                    printFrame(frame, "RX OK: ");
                }
            };

            CommunicationUnit[] list = Communication.getArrayOfUnit();

            foreach (CommunicationUnit c in list)
            {
                channel.BasicConsume(queue:     RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                     noAck:     true,
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
            CommunicationUnit[] list = Communication.getArrayOfUnit();

            foreach (CommunicationUnit c in list)
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
            CommunicationUnit[] list = Communication.getArrayOfUnit();

            foreach (CommunicationUnit c in list)
            {
                channel.QueueBind(  queue:      RABBITMQ.CMD_QUEUE + c.Address.ToString("D5"), 
                                    exchange:   RABBITMQ.EXCHANGE_CMD, 
                                    routingKey: c.Address.ToString("D5"));

                channel.QueueBind(  queue:      RABBITMQ.TLM_QUEUE + c.Address.ToString("D5"),
                                    exchange:   RABBITMQ.EXCHANGE_ANS,
                                    routingKey: c.Address.ToString("D5"));
            }
        }

        private void processTx()
        {
            try
            {
                if (stopTx.Elapsed.Milliseconds > 100)
                {
                    if (Communication.isAnyQueueCmd())
                    {
                        Cmd cmd             = Communication.getNextCmd();
                        cmd.Header.Count    = count++;
                        DataFrame frame     = new DataFrame(cmd.Header,cmd.Payload);

                        publishCMD(frame);

                        Communication.removeCmd(cmd);
                        Communication.addTxCmd(cmd);

                        printTx(frame, "TX OK: ");
                        stopTx.Restart();
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Transmissao AMQP");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
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

        private void publishCMD(DataFrame frame)
        {
            var body = Encoding.UTF8.GetBytes(frame.Data);

            channel.BasicPublish(   exchange:           RABBITMQ.EXCHANGE_CMD,
                                    routingKey:         frame.Header.Dest.ToString("D5"),
                                    basicProperties:    null,
                                    body:               body);
        }

        private void printFrame(DataFrame frame, string str)
        {
            Debug.WriteLine(str);
            foreach (char c in frame.Data)
                Debug.Write(c.ToString());
            Debug.Write("\r\n");
        }

        private void printTx(DataFrame frame, string str)
        {
            Debug.WriteLine("TX TO:{0}[{1}] {2}{3} ms", frame.Header.Resource, frame.Header.Count.ToString("D5"), stopTx.Elapsed.Seconds.ToString("D2"), stopTx.Elapsed.Milliseconds.ToString("D3"));
            printFrame(frame, str);
        }

        // Para testes
        public void publishAnswer(DataFrame frame)
        {
            var body = Encoding.UTF8.GetBytes(frame.Data);

            channel.BasicPublish(exchange: RABBITMQ.EXCHANGE_ANS,
                                    routingKey: frame.Header.Address.ToString("D5"),
                                    basicProperties: null,
                                    body: body);

        }
    }
}
