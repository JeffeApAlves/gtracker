using System.Collections.Generic;

namespace GoodsTracker
{
    abstract class CommunicationUnit
    {
        /*
         * Endereco da unidade
         */
        int     address;

        private static Dictionary<string, Cmd> containner = new Dictionary<string, Cmd>();
        private static List<Cmd>                queueCmd = new List<Cmd>();
        private static List<AnsCmd>             queueAnsCmd = new List<AnsCmd>();

        internal List<Cmd> QueueCmd { get => queueCmd; set => queueCmd = value; }
        internal List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }
        internal int Address { get => address; set => address = value; }

        internal CommunicationUnit()
        {
            Protocol.Units.Add(this);
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

        internal Cmd sendCMD(int dest, Operation o,string resource)
        {
            Cmd c       = new Cmd(resource);
            c.Dest      = dest;
            c.Operation = 0;
            c.Resource  = resource;

            queueCmd.Add(c);

            return c;
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
                foreach (AnsCmd ans in queueAnsCmd)
                {
                    foreach (Cmd cmd in queueCmd)
                    {
                        if (ans.Resource == cmd.Resource)
                        {
                            if (cmd.CallBackAns(ans) == ResultExec.EXEC_SUCCESS)
                            {
                                removeCmd(cmd);
                            }
                        }
                    }
                }
            }
        }

    }
}
