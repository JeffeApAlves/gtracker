using System.Collections.Generic;

namespace GoodsTracker
{
    abstract class CommunicationUnit
    {
        /*
         * Endereco da unidade
         */
        int     address;

        private static Dictionary<IdCmd, Cmd>   containner = new Dictionary<IdCmd, Cmd>();
        private static List<Cmd>                queueCmd = new List<Cmd>();
        private static List<AnsCmd>             queueAnsCmd = new List<AnsCmd>();

        internal List<Cmd> QueueCmd { get => queueCmd; set => queueCmd = value; }
        internal List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }
        internal int Address { get => address; set => address = value; }

        internal CommunicationUnit()
        {
            Protocol.Units.Add(this);
        }

        internal bool isAnyCmd()
        {
            return queueCmd.Count > 0;
        }

        internal Cmd getNextCmd()
        {
            return isAnyCmd()? queueCmd[0]:null;
        }

        internal bool isAnyAns()
        {
            return queueAnsCmd.Count > 0;
        }

        internal void addAns(AnsCmd ans)
        {
            queueAnsCmd.Add(ans);
        }

        internal void removeCmd(Cmd cmd)
        {
            queueCmd.Remove(cmd);
        }

        internal void sendCMD(IdCmd id, CallBackAnsCmd ans)
        {
            Cmd c = new Cmd(id);

            c.setCallBack(ans);
        }

        internal static Cmd getCMD(IdCmd id_cmd)
        {
            return containner[id_cmd];
        }

        internal static Cmd findCMD(string name)
        {
            foreach (var item in containner)
            {
                if (item.Value.getName() == name)
                {
                    return item.Value;
                }
            }

            return null;
        }

        internal void processQueue()
        {
            if (isAnyAns())
            {
                foreach (AnsCmd ans in QueueAnsCmd)
                {
                    foreach (Cmd cmd in QueueCmd)
                    {
                        if (ans.NameCmd == cmd.getName())
                        {
                            if (cmd.CallBackAns(ans.DadosRx) == ResultExec.EXEC_SUCCESS)
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
