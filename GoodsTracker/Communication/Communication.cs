using System;
using System.Diagnostics;
using System.Collections.Generic;

namespace GoodsTracker
{
    delegate void acceptAnswerHandler(AnsCmd ans);

    enum TYPE_COMMUNICATION
    {
        SERIAL,AMQP
    }

    abstract class Communication : ThreadRun, Communic
    {
        private const int _TIME_COMMUNICATION = 1;

        private static TYPE_COMMUNICATION type;
        private static Dictionary<int,DeviceBase> devices = new Dictionary<int,DeviceBase>();
        private static Dictionary<string, Cmd> txCmds = new Dictionary<string, Cmd>();
        private static Dictionary<string, Cmd> cmds = new Dictionary<string, Cmd>();
        private static List<AnsCmd> queueAnsCmd = new List<AnsCmd>();
        internal static TYPE_COMMUNICATION Type { get => type; set => type = value; }
        internal static int count = 0;
        public static int TIME_COMMUNICATION => _TIME_COMMUNICATION;
        internal static Dictionary<int, DeviceBase> Devices { get => devices; set => devices = value; }
        internal static Dictionary<string, Cmd> TxCmds { get => txCmds; set => txCmds = value; }
        internal static Dictionary<string, Cmd> Cmds { get => cmds; set => cmds = value; }
        internal static List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }

        private Stopwatch stopTx = new Stopwatch();

        public Communication()
        {
            setTime(_TIME_COMMUNICATION);
        }

        internal static void addDevice(DeviceBase device)
        {
            if (device != null)
            {
                devices[device.Address] = device;
            }
        }

        public override void run()
        {
            processTx();
            processRx();
        }

        public static Communic create(TYPE_COMMUNICATION t)
        {
            type = t;

            Communic communic = null;

            switch (type)
            {
                case TYPE_COMMUNICATION.SERIAL: communic    = new SerialCommunication(); break;
                case TYPE_COMMUNICATION.AMQP: communic      = new AMPQCommunication(); break;
            }

            return communic;
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
            Cmd cmd = null;

            if (txCmds.ContainsKey(ans.Header.Resource)) {

                cmd = txCmds[ans.Header.Resource];

                if ((ans.Header.Dest != cmd.Header.Address)/* || 
                (ans.Header.Count!=cmd.Header.Count)*/)
                {
                    cmd = null;
                }
            }

            return cmd;
        }

        public static DeviceBase[] getArrayOfDevices()
        {
            DeviceBase[] ret = null;

            if (devices.Count > 0)
            {
                ret = new DeviceBase[devices.Count];

                devices.Values.CopyTo(ret,0);
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

        internal static void acceptAnswerDevice(Cmd cmd, AnsCmd ans)
        {
            if (devices.ContainsKey(ans.Header.Address))
            {
                devices[ans.Header.Address].acceptAnswer(cmd, ans);
            }
        }

        private void processTx()
        {
            try
            {
                if (stopTx.Elapsed.Seconds > 1)
                {
                    if (isAnyQueueCmd())
                    {
                        Cmd cmd = getNextCmd();
                        cmd.Header.Count = count++;

                        if (sendCmd(cmd))
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
                // Implementada pela classe especialista
                receive();
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro no processamento da pilha de Recepcao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }

        public void Start()
        {
            stopTx.Start();
            Init();
        }

        public void Stop()
        {
            RequestStop();
            join();
            DeInit();
        }

        public virtual void register(DeviceBase device)
        {
            // Adiciona no container
            addDevice(device);

            // Atribui a interface de comunicacaoque sera usada pelo periferico
            device.Communic = this;
        }

        public virtual bool send(Cmd cmd)
        {
            Communication.addCmd(cmd);

            return true;
        }

        public virtual bool send(AnsCmd ans)
        {
            return sendAns(ans);
        }

        //Debug
        protected void printFrame(string str, DataFrame frame)
        {
            Debug.Write(str + ": ");
            foreach (char c in frame.Data)
                Debug.Write(c.ToString());
            Debug.Write("\r\n");
        }

        //Debug
        protected void printTx(string str, DataFrame frame)
        {
            Debug.WriteLine(str + ": {0}[{1}] {2}{3} ms", frame.Header.Resource, frame.Header.Count.ToString("D5"), stopTx.Elapsed.Seconds.ToString("D2"), stopTx.Elapsed.Milliseconds.ToString("D3"));
            printFrame(str, frame);
        }

        //Debug
        protected void printRx(string str, DataFrame frame)
        {
            Debug.WriteLine(str + ": {0}[{1}]", frame.Header.Resource, frame.Header.Count.ToString("D5"));
            printFrame(str, frame);
        }

        //Implementação que será feita pela classe especialista
        public abstract void Init();
        public abstract void DeInit();
        public abstract bool sendCmd(Cmd cmd);
        public abstract bool sendAns(AnsCmd ans);
        public abstract bool receive();
    }
}
