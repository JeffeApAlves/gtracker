using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;

namespace GoodsTracker
{
    delegate void acceptAnswerHandler(AnsCmd ans);

    enum TYPE_COMMUNICATION
    {
        SERIAL,AMQP
    }

    abstract class Communication : ThreadRun, Communic
    {
        private const int _TIME_COMMUNICATION   = 1;
        private const int TIME_BETWEEN_CMD      = 25;

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
                case TYPE_COMMUNICATION.SERIAL: communic    = new SerialCommunication();    break;
                case TYPE_COMMUNICATION.AMQP:   communic    = new AMPQCommunication();      break;
            }

            return communic;
        }

        public static FrameSerialization createSerialization()
        {
            FrameSerialization serialization = null;

            switch (type)
            {
                case TYPE_COMMUNICATION.SERIAL: serialization   = new SerialSerialization(); break;
                case TYPE_COMMUNICATION.AMQP:   serialization   = new SerialSerialization(); break;
            }

            return serialization;
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

                if ((ans.Header.Dest != cmd.Header.Address))
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
                if (stopTx.Elapsed.Milliseconds > TIME_BETWEEN_CMD)
                {
                    if (isAnyQueueCmd())
                    {
                        try
                        {
                            Cmd cmd = getNextCmd();
                            cmd.Header.TimeStamp = (Int32)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;

                            sendCmd(cmd);
                            addTxCmd(cmd);
                            removeCmd(cmd);
                        }
                        catch (Exception e)
                        {
                            Debug.WriteLine(e);
                        }

                        stopTx.Restart();
                    }
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine("Erro no processamento da pilha de Transmissao");
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
                Debug.WriteLine("Erro no processamento da pilha de Recepcao");
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

            // Atribui a interface de comunicação que sera usada pelo periférico
            device.Communic = this;
        }

        public virtual bool send(Cmd cmd)
        {
            Communication.addCmd(cmd);

            return true;
        }

        public virtual bool send(AnsCmd ans)
        {
            bool ret = false;

            try
            {
                sendAns(ans);

                ret = true;
            }
            catch (Exception e)
            {
                Debug.WriteLine(e);
            }

            return ret;
        }

        // Debug
        protected void printTxFrame(string str, CommunicationFrame frame)
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(str + " :");
            sb.Append(" " + stopTx.Elapsed.Seconds.ToString("D2") + stopTx.Elapsed.Milliseconds.ToString("D3") + " ms");
            sb.Append(" " + frame.Header.Resource);
            //sb.Append(" ["+ frame.Header.TimeStamp.ToString("D10")+"] ");

            foreach (char c in frame.Data)
            {
                sb.Append(c.ToString());
            }
 
            Debug.WriteLine(sb);

        }

        // Debug
        protected void printRxFrame(string str, CommunicationFrame frame)
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(str + " :");
            sb.Append(" " + frame.Header.Resource);
            sb.Append(" [" + frame.Header.TimeStamp.ToString("D10") + "] ");

            foreach (char c in frame.Data)
            {
                sb.Append(c.ToString());
            }

            Debug.WriteLine(sb);
        }

        // Implementação que será feita pela classe especialista
        public abstract void Init();
        public abstract void DeInit();
        public abstract bool sendCmd(Cmd cmd);
        public abstract bool sendAns(AnsCmd ans);
        public abstract bool receive();
    }
}
