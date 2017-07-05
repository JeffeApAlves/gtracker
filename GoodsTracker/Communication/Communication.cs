using System;
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
        void publishAnswer(DataFrame frame);
    }

    abstract class Communication : ThreadRun
    {
        private static TYPE_COMMUNICATION type;
        private static Communic communic = null;
        private const int _TIME_COMMUNICATION = 1;
        private static Dictionary<int,CommunicationUnit> units = new Dictionary<int,CommunicationUnit>();
        private static Dictionary<string, Cmd> txCmds = new Dictionary<string, Cmd>();
        private static Dictionary<string, Cmd> cmds = new Dictionary<string, Cmd>();
        private static List<AnsCmd> queueAnsCmd = new List<AnsCmd>();

        internal static Dictionary<int,CommunicationUnit> Units { get => units; set => units = value; }
        internal static TYPE_COMMUNICATION Type { get => type; set => type = value; }
        internal static List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }
        internal static Dictionary<string, Cmd> TxCmds { get => txCmds; set => txCmds = value; }
        internal static Dictionary<string, Cmd> Cmds { get => cmds; set => cmds = value; }

        public Communication()
        {
            setTime(_TIME_COMMUNICATION);
        }

        internal static void AddUnit(CommunicationUnit unit)
        {
            if (unit != null)
            {
                units[unit.Address] = unit;
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

        public static void create(TYPE_COMMUNICATION t)
        {
            if (type != t || communic == null)
            {
                type = t;
                DeInit();

                switch (type)
                {
                    case TYPE_COMMUNICATION.SERIAL: communic = new SerialCommunication(); break;
                    case TYPE_COMMUNICATION.AMQP: communic = new AMQPCommunication(); break;
                }

                Init();
            }
        }

        public static bool isAnyTxCmd()
        {
            return txCmds.Count > 0;
        }


        public static bool isAnyAns()
        {
            return queueAnsCmd.Count > 0;
        }

        public static bool isAnyQueueCmd()
        {
            return cmds.Count > 0;
        }

        public static void addAns(AnsCmd ans)
        {
            if (ans != null)
            {
                queueAnsCmd.Add(ans);
            }
        }

        public static void removeAns(AnsCmd ans)
        {
            if (ans != null)
            {
                queueAnsCmd.Remove(ans);
            }
        }

        public static void removeTxCmd(Cmd cmd)
        {
            if (cmd != null)
            {
                txCmds.Remove(cmd.Header.Resource);
            }
        }

        public static void addTxCmd(Cmd cmd)
        {
            if (cmd != null)
            {
                txCmds[cmd.Header.Resource] = cmd;
            }
        }

        public static void removeCmd(Cmd cmd)
        {
            if (cmd != null)
            {
                cmds.Remove(cmd.Header.Resource);
            }
        }

        public static void addCmd(Cmd cmd)
        {
            if (cmd != null)
            {
                cmds[cmd.Header.Resource] = cmd;
            }
        }

        public static Cmd searchCmdOfAnswer(AnsCmd ans)
        {
            Cmd cmd = txCmds[ans.Header.Resource];

            if ((ans.Header.Resource != cmd.Header.Resource) ||
                (ans.Header.Dest != cmd.Header.Address) || 
                (ans.Header.Count!=cmd.Header.Count))
            {
                cmd = null;
            }

            return cmd;
        }

        public static CommunicationUnit[] getArrayOfUnit()
        {
            CommunicationUnit[] ret = null;

            if (units.Count > 0)
            {
                ret = new CommunicationUnit[units.Count];

                units.Values.CopyTo(ret,0);
            }

            return ret;
        }

        public static void publishAnswer(DataFrame frame)
        {
            communic.publishAnswer(frame);
        }

        internal static void acceptAnswer(AnsCmd ans)
        {
            if (ans != null)
            {
                Cmd cmd = searchCmdOfAnswer(ans);

                addAns(ans);

                units[ans.Header.Address].processAnswer(ans);

                removeTxCmd(cmd);
                removeAns(ans);
            }
        }

        internal static Cmd getNextCmd()
        {
            Cmd cmd = null;

            if (isAnyQueueCmd())
            {
                List<Cmd> tmp = new List<Cmd>(Cmds.Values);

                cmd = tmp[0];
            }

            return cmd;
        }
    }

    /****************************************************************************************
     * Comunicacao via Serial
     ****************************************************************************************/
    internal class SerialCommunication : Communication , Communic
    {
        static Protocol serialProtocol   = null;
        static Serial   serialPort       = null;

        public void doCommunication()
        {
            serialProtocol.doCommunication();
        }

        public new void Init()
        {
            serialPort = new Serial();
            serialPort.Open();

            if (serialProtocol == null)
            {
                serialProtocol = new Protocol(serialPort);
            }

            start();
        }

        public new void DeInit()
        {
            RequestStop();
            join();
            serialPort.Close();
        }

        public new void publishAnswer(DataFrame frame)
        {
            serialProtocol.publishAnswer(frame);
        }
    }

    /*************************************************************************************
     * Comunicacao via AMQP
     *************************************************************************************/ 
    internal class AMQPCommunication : Communication, Communic
    {
        static RabbitMQ amqp = null;

        public void doCommunication()
        {
            amqp.doCommunication();
        }

        public new void Init()
        {
            if (amqp == null)
            {
                amqp = new RabbitMQ();
            }

            amqp.Open();
            start();
        }

        public new void DeInit()
        {
            RequestStop();
            join();
            amqp.Close();
        }

        public new void publishAnswer(DataFrame frame)
        {
            amqp.publishAnswer(frame);
        }
    }
}
