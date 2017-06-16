using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    enum ResultExec
    {
        EXEC_UNSUCCESS  = -3,
        INVALID_CMD     = -2,
        INVALID_PARAM   = -1,
        EXEC_SUCCESS    = 0,
    };
    
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

        IdCmd   idCmd;
        int     address;
        int     value;

        public int Address { get => address; set => address = value; }
        public int Value { get => value; set => this.value = value; }
        internal IdCmd IdCmd { get => idCmd; set => idCmd = value; }

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

        internal ResultExec callBack(ParamCmd param)
        {
            return ResultExec.EXEC_SUCCESS;
        }
    }
}
