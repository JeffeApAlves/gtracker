using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace GoodsTracker
{
    delegate void acceptAnswerHandler(AnsCmd ans);

    enum TYPE_COMMUNICATION
    {
        SERIAL,
        AMQP
    }

    interface Communic
    {
        void Init();
        void DeInit();
        bool receive();
        void send(DataFrame frame);
        bool send(Cmd cmd);
    }

    abstract class Communication : ThreadRun, Communic
    {
        private static TYPE_COMMUNICATION type;
        private static Communic communic = null;
        private const int _TIME_COMMUNICATION = 1;
        private static Dictionary<int,BaseCommunication> units = new Dictionary<int,BaseCommunication>();
        private static Dictionary<string, Cmd> txCmds = new Dictionary<string, Cmd>();
        private static Dictionary<string, Cmd> cmds = new Dictionary<string, Cmd>();
        private static List<AnsCmd> queueAnsCmd = new List<AnsCmd>();

        internal static TYPE_COMMUNICATION Type { get => type; set => type = value; }
        internal static Communic Communic { get => communic; set => communic = value; }
        internal static int count = 0;
        public static int TIME_COMMUNICATION => _TIME_COMMUNICATION;

        internal static Dictionary<int, BaseCommunication> Units { get => units; set => units = value; }
        internal static Dictionary<string, Cmd> TxCmds { get => txCmds; set => txCmds = value; }
        internal static Dictionary<string, Cmd> Cmds { get => cmds; set => cmds = value; }
        internal static List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }

        Stopwatch stopTx = new Stopwatch();

        public Communication()
        {
            setTime(_TIME_COMMUNICATION);
        }

        internal static void AddUnit(BaseCommunication unit)
        {
            if (unit != null)
            {
                units[unit.Address] = unit;
            }
        }

        public override void run()
        {
            processTx();
            processRx();
        }

        public static void create(TYPE_COMMUNICATION t)
        {
            if (type != t || communic == null)
            {
                type = t;

                if (communic != null)
                {
                    communic.DeInit();
                }

                switch (type)
                {
                    case TYPE_COMMUNICATION.SERIAL: communic = new SerialCommunication(); break;
                    case TYPE_COMMUNICATION.AMQP: communic = new AMPQCommunication(); break;
                }
                if (communic != null)
                {
                    communic.Init();
                }
                else
                {
                    Debug.WriteLine("Problema na inicializacao da comunicação");
                }
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

        public static BaseCommunication[] getArrayOfUnit()
        {
            BaseCommunication[] ret = null;

            if (units.Count > 0)
            {
                ret = new BaseCommunication[units.Count];

                units.Values.CopyTo(ret,0);
            }

            return ret;
        }

        internal static Cmd getNextCmd()
        {
            Cmd cmd = null;

            if (isAnyQueueCmd())
            {
                List<Cmd> tmp = new List<Cmd>(cmds.Values);

                cmd = tmp[0];
            }

            return cmd;
        }

        internal static void processAnswer(Cmd cmd, AnsCmd ans)
        {
            if (units.ContainsKey(ans.Header.Address))
            {
                units[ans.Header.Address].processAnswer(cmd, ans);
            }
        }

        public virtual void DeInit()
        {
            RequestStop();
            join();
        }

        private void processTx()
        {
            try
            {
                if (stopTx.Elapsed.Seconds > 2)
                {
                    if (isAnyQueueCmd())
                    {
                        Cmd cmd = getNextCmd();
                        cmd.Header.Count = count++;

                        if (send(cmd))
                        {
                            removeCmd(cmd);
                            addTxCmd(cmd);
                        }

                        stopTx.Restart();
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Transmissao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        private void processRx()
        {
            try
            {
                receive();
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Recepcao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        public virtual void Init()
        {
            stopTx.Start();
        }

        public static void sendFrame(DataFrame frame)
        {
            if (communic != null)
            {
                communic.send(frame);
            }
        }

         public static void stop()
        {
            if (communic != null)
            {
                communic.DeInit();
            }
        }

        internal void printFrame(string str, DataFrame frame)
        {
            Debug.Write(str+": ");
            foreach (char c in frame.Data)
                Debug.Write(c.ToString());
            Debug.Write("\r\n");
        }

        internal void printTx(string str, DataFrame frame)
        {
            Debug.WriteLine("TX: {0}[{1}] {2}{3} ms", frame.Header.Resource, frame.Header.Count.ToString("D5"), stopTx.Elapsed.Seconds.ToString("D2"), stopTx.Elapsed.Milliseconds.ToString("D3"));
            printFrame(str, frame);
        }

        public abstract bool receive();
        public abstract bool send(Cmd cmd);
        public abstract void send(DataFrame frame);
    }
}
