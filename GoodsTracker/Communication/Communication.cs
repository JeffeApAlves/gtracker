using System.Collections.Generic;

namespace GoodsTracker
{
    enum TYPE_COMMUNICATION
    {
        SERIAL,
        AMQP
    }

    interface Communic
    {
        void Init();
        void DeInit();
        void doCommunication();
    }

    abstract class Communication : ThreadRun
    {
        static Communic communic = null;
        private const int _TIME_COMMUNICATION = 1;
        static List<CommunicationUnit> units = new List<CommunicationUnit>();

        internal static List<CommunicationUnit> Units { get => units; set => units = value; }

        public Communication()
        {
            setTime(_TIME_COMMUNICATION);
        }

        internal static void AddUnit(CommunicationUnit unit)
        {
            units.Add(unit);
        }

        internal static void processQueues()
        {
            foreach(CommunicationUnit u in units)
            {
                u.processQueues();
            }
        }

        public override void run()
        {
            if (communic != null)
            {
                communic.doCommunication();
            }            
        }

        internal static void Init()
        {
            if (communic != null)
            {
                communic.Init();
            }
        }

        public static void DeInit()
        {
            if (communic != null)
            {
                communic.DeInit();
            }
        }

        public static void create(TYPE_COMMUNICATION type)
        {
            switch (type)
            {
                case TYPE_COMMUNICATION.SERIAL: communic = new SerialCommunication();   break;
                case TYPE_COMMUNICATION.AMQP:   communic = new AMQPCommunication();     break;
            }
            
            Init();
        }
    }

    internal class SerialCommunication : Communication , Communic
    {
        static Protocol serialProtocol   = null;
        static Serial   serialPort       = null;

        public void doCommunication()
        {
            serialProtocol.processTx();
            serialProtocol.processRx();
            processQueues();
        }

        public void Init()
        {
            serialPort = new Serial();
            serialPort.Open();

            if (serialProtocol == null)
            {
                serialProtocol = new Protocol(serialPort);
            }
        }

        public void DeInit()
        {
            serialPort.Close();
        }

        internal static void writeRx(DataFrame frame)
        {
            serialProtocol.writeRx(frame);
        }
    }

    internal class AMQPCommunication : Communication, Communic
    {
        static RabbitMQ amqp = null;

        public void doCommunication()
        {
            amqp.processTx();
            amqp.processRx();
        }

        public void Init()
        {
            amqp = new RabbitMQ();
            amqp.Open();
        }

        public void DeInit()
        {
            amqp.Close();
        }
    }
}
