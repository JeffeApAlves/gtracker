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

        IdCmd   id_cmd;

        private Cmd()
        {
        }

        private Cmd(IdCmd id)
        {
            id_cmd = id;

            Containner.Add(id, this);
        }

        public static Cmd createCMD(IdCmd id)
        {
            return new Cmd(id);
        }

        public static Cmd getCMD(IdCmd id_cmd)
        {
            return Containner[id_cmd];
        }

        public static Cmd findCMD(string name)
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

        public string getName()
        {
            string name_cmd = "";

            switch (id_cmd)
            {
                case IdCmd.CMD_LED:     name_cmd = "LED";   break;
                case IdCmd.CMD_ANALOG:  name_cmd = "AN";    break;
                case IdCmd.CMD_PWM:     name_cmd = "PWM";   break;
                case IdCmd.CMD_TOUCH:   name_cmd = "ACC";   break;
                case IdCmd.CMD_ACC:     name_cmd = "TOU";   break;
            }

            return name_cmd;
        }

        public ResultExec callBack(ParamCmd param)
        {
            return ResultExec.EXEC_SUCCESS;
        }

        internal object GetAddress()
        {
            throw new NotImplementedException();
        }

        internal object GetValue()
        {
            throw new NotImplementedException();
        }
    }
}
