using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    public class RESOURCE
    {
        public static string BEHAVIOR   = "DB1";    //Data Behavior
        public static string LOCK       = "LOCK";
    }

    class Cmd
    {
        onAnswerCmd onAnswerCmd;

        int         dest;
        int         address;
        int         value;
        Operation   operation;
        string      resource;
    
        public int Address { get => address; set => address = value; }
        public int Value { get => value; set => this.value = value; }
        public Operation Operation { get => operation; set => operation = value; }
        public int Dest { get => dest; set => dest = value; }
        public onAnswerCmd EventAnswerCmd { get => onAnswerCmd; set => onAnswerCmd = value; }
        public string Resource { get => resource; set => resource = value; }

        internal Cmd(string r)
        {
            resource = r;
        }

        public void setEventAnswerCmd(onAnswerCmd ans)
        {
            onAnswerCmd = ans;
        }
    }
}
