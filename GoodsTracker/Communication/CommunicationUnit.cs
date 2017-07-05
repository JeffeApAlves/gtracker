using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace GoodsTracker
{
    public enum ResultExec
    {
        EXEC_UNSUCCESS  = -3,
        INVALID_CMD     = -2,
        INVALID_PARAM   = -1,
        EXEC_SUCCESS    = 0,
    };

    internal delegate ResultExec onAnswerCmd(AnsCmd ans);

    abstract class CommunicationUnit
    {
        // Server
        private const int MASTER_NUMBER = 1;

        /*
         * Endereco da unidade
         */
        protected int   address;

        private static Dictionary<string, Cmd>  txCmds  = new Dictionary<string, Cmd>();
        private static Dictionary<string, Cmd>  cmds    = new Dictionary<string, Cmd>();
        private static List<AnsCmd>             queueAnsCmd = new List<AnsCmd>();

        internal int Address { get => address; set => address = value; }
        public static List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }
        internal static Dictionary<string, Cmd> TxCmds { get => txCmds; set => txCmds = value; }
        internal static Dictionary<string, Cmd> Cmds { get => cmds; set => cmds = value; }

        protected abstract void onReceiveAnswer(AnsCmd ans);

        internal CommunicationUnit()
        {
            Communication.AddUnit(this);
        }

        static internal bool isAnyTxCmd()
        {
            return txCmds.Count > 0;
        }

        static internal bool isAnyQueueCmd()
        {
            return cmds.Count > 0;
        }

        internal static Cmd getNextCmd()
        {
            Cmd cmd = null;

            if (isAnyQueueCmd())
            {
                List<Cmd> tmp = new List<Cmd>(cmds.Values);

                cmd = tmp[0];

                txCmds[cmd.Header.Resource] = cmd;

                cmds.Remove(cmd.Header.Resource);
            }
            
            return cmd;
        }

        static internal bool isAnyAns()
        {
            return queueAnsCmd.Count > 0;
        }

        static internal void addAns(AnsCmd ans)
        {
            queueAnsCmd.Add(ans);
        }

        internal Cmd createCMD(int dest, Operation o, string resource)
        {
            Cmd c = new Cmd(resource, o);
            c.Header.Dest       = dest;
            c.Header.Address    = MASTER_NUMBER;

            return c;
        }

        internal void sendCMD(Cmd cmd)
        {
            cmds[cmd.Header.Resource] =cmd;
        }

        internal void processQueues()
        {
            if (isAnyTxCmd() && isAnyAns())
            {
                Cmd[] array_cmd = new List<Cmd>(txCmds.Values).ToArray();

                foreach (Cmd cmd in array_cmd)
                {
                    AnsCmd[] array_ans = queueAnsCmd.ToArray();

                    foreach (AnsCmd ans in array_ans)
                    {
                        if ((ans.Header.Resource == cmd.Header.Resource) && 
                            (ans.Header.Dest == cmd.Header.Address))
                        {
                            try
                            {
                                // Executa evento de recebmento de resposta de comando
                                onReceiveAnswer(ans);

                                // Executa call back respectiva do comando
                                cmd.EventAnswerCmd?.Invoke(ans);

                                removeCmd(cmd);
                                removeAns(ans);
                            }
                            catch (Exception e)
                            {
                                Console.WriteLine("Erro na execucao das callbacks de communicacao");
                                Console.WriteLine(e.ToString());
                                Debug.WriteLine(e.ToString());
                            }

                            break;
                        }
                    }
                }
            }
        }

        static internal void removeCmd(Cmd cmd)
        {
            txCmds.Remove(cmd.Header.Resource);
        }

        private void removeAns(AnsCmd ans)
        {
            queueAnsCmd.Remove(ans);
        }
    }
}
