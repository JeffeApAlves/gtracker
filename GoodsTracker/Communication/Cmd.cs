using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    enum IdCmd
    {
        CMD_LED,
        CMD_ANALOG,
        CMD_PWM,
        CMD_TOUCH,
        CMD_ACC
    };

    class Cmd
    {
        private static Dictionary<IdCmd, Cmd> Containner = new Dictionary<IdCmd, Cmd>();

        ansCmd  ansCmd;

        IdCmd   idCmd;
        int     dest;
        int     address;
        int     value;
        Operation operation;


        public int Address { get => address; set => address = value; }
        public int Value { get => value; set => this.value = value; }
        internal IdCmd IdCmd { get => idCmd; set => idCmd = value; }
        public Operation Operation { get => operation; set => operation = value; }
        public int Dest { get => dest; set => dest = value; }


        internal Cmd()
        {
        }

        internal Cmd(IdCmd id)
        {
            idCmd = id;

            Containner.Add(id, this);
        }

        internal static Cmd createCMD(IdCmd id)
        {
            return new Cmd(id);
        }

        internal static Cmd getCMD(IdCmd id_cmd)
        {
            return Containner[id_cmd];
        }

        internal static Cmd findCMD(string name)
        {
            foreach (var item in Containner)
            {
                if (item.Value.getName() == name)
                {
                    return item.Value;
                }
            }

            return null;
        }

        internal string getName()
        {
            string name_cmd = "";

            switch (idCmd)
            {
                case IdCmd.CMD_LED:     name_cmd = "LED";   break;
                case IdCmd.CMD_ANALOG:  name_cmd = "AN";    break;
                case IdCmd.CMD_PWM:     name_cmd = "PWM";   break;
                case IdCmd.CMD_TOUCH:   name_cmd = "ACC";   break;
                case IdCmd.CMD_ACC:     name_cmd = "TOU";   break;
            }

            return name_cmd;
        }

        internal void setCallBackAnsCmd(ansCmd ans)
        {
            ansCmd = ans;
        }
    }
}
