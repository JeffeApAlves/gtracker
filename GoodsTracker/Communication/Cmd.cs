using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{/*
    enum IdCmd
    {        
        CMD_NONE = 0,
        CMD_BEHAVIOR = 1,
        CMD_LOCK = 2
    };*/

        
    public class RESOURCE
    {
        public static string BEHAVIOR   = "DATA1";
        public static string LOCK       = "LOCK";
    }

    class Cmd
    {
        CallBackAnsCmd      callBackAns;

//        IdCmd       idCmd;
        int         dest;
        int         address;
        int         value;
        Operation   operation;
        string      resource;
    
        public int Address { get => address; set => address = value; }
        public int Value { get => value; set => this.value = value; }
//        internal IdCmd IdCmd { get => idCmd; set => idCmd = value; }
        public Operation Operation { get => operation; set => operation = value; }
        public int Dest { get => dest; set => dest = value; }
        public CallBackAnsCmd CallBackAns { get => callBackAns; set => callBackAns = value; }
        public string Resource { get => resource; set => resource = value; }

        internal Cmd(string r)
        {
            resource = r;
        }

/*        internal Cmd(IdCmd id)
        {
            idCmd = id;
        }*/

/*        internal static Cmd createCMD(IdCmd id)
        {
            return new Cmd(id);
        }*/

        public void setCallBack(CallBackAnsCmd ans)
        {
            callBackAns = ans;
        }
    }
}
