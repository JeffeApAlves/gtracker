using System.Collections.Generic;

namespace GoodsTracker
{
    internal delegate ResultExec onAnswerCmd(AnsCmd ans);

    abstract class CommunicationUnit
    {
        /*
         * Endereco da unidade
         */
        protected int   address;

        static int      index = 0;

        static List<CommunicationUnit>          units = new List<CommunicationUnit>();
        private static Dictionary<string, Cmd>  containner = new Dictionary<string, Cmd>();
        private static List<Cmd>                queueCmd = new List<Cmd>();
        private static List<AnsCmd>             queueAnsCmd = new List<AnsCmd>();

        internal int Address { get => address; set => address = value; }
        public static List<Cmd> QueueCmd { get => queueCmd; set => queueCmd = value; }
        public static List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }

        protected abstract void onReceiveAnswer(AnsCmd ans);

        internal CommunicationUnit()
        {
            units.Add(this);
        }

        static internal bool isAnyCmd()
        {
            return queueCmd.Count > 0;
        }

        internal Cmd getNextCmd()
        {
            return isAnyCmd()? queueCmd[0]:null;
        }

        static internal bool isAnyAns()
        {
            return queueAnsCmd.Count > 0;
        }

        static internal void addAns(AnsCmd ans)
        {
            queueAnsCmd.Add(ans);
        }

        static internal void removeCmd(Cmd cmd)
        {
            queueCmd.Remove(cmd);
        }

        internal Cmd createCMD(int dest, Operation o, string resource)
        {
            Cmd c = new Cmd(resource, o);
            c.Header.Dest = dest;
            c.Header.Address = address;

            return c;
        }

        internal void sendCMD(Cmd cmd)
        {
            queueCmd.Add(cmd);
        }

        internal static CommunicationUnit getNextUnit()
        {
            return units[(index++) % units.Count];
        }

        static internal Cmd getCMD(string resource)
        {
            return containner[resource];
        }

        static internal Cmd findCMD(string resource)
        {
            return containner[resource];
        }

        internal void processQueue()
        {
            if (isAnyAns())
            {
                Cmd[]       array_cmd = queueCmd.ToArray();

                foreach (Cmd cmd in array_cmd)
                {
                    AnsCmd[] array_ans = queueAnsCmd.ToArray();

                    foreach (AnsCmd ans in array_ans)
                    {
                        if ((ans.Header.Resource == cmd.Header.Resource) && (ans.Header.Dest ==cmd.Header.Address))
                        {
                            try
                            {
                                // Executa evento de recebmento de resposta de comando
                                onReceiveAnswer(ans);

                                // Excuta call back respectiva do comando
                                cmd.EventAnswerCmd?.Invoke(ans);

                                removeCmd(cmd);
                                removeAns(ans);
                            }
                            catch
                            {

                            }

                            break;
                        }
                    }
                }
            }
        }

        private void removeAns(AnsCmd ans)
        {
            queueAnsCmd.Remove(ans);
        }
    }
}
