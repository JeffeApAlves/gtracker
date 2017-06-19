using System.Collections.Generic;

namespace GoodsTracker
{
    abstract class CommunicationUnit : IUpdateCommUnit
    {
        /*
         * Endereco da unidade
         */
        int     address;

        List<Cmd>       queueCmd = new List<Cmd>();
        List<AnsCmd>    queueAnsCmd = new List<AnsCmd>();

        internal List<Cmd> QueueCmd { get => queueCmd; set => queueCmd = value; }
        internal List<AnsCmd> QueueAnsCmd { get => queueAnsCmd; set => queueAnsCmd = value; }
        internal int Address { get => address; set => address = value; }

        public abstract void update(ObjectValueRX dados);

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
    }
}
