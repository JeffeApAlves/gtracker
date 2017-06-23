using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    public class RESOURCE
    {
        public const string TELEMETRIA  = "TLM";    // Data TelemetriaData
        public const string LOCK        = "LCK";    // Trava 
        public const string LCD         = "LCD";
    }

    class Cmd
    {
        onAnswerCmd onAnswerCmd;

        int         dest;
        int         address;
        Operation   operation;
        string      resource;
    
        public int Address { get => address; set => address = value; }
        public Operation Operation { get => operation; set => operation = value; }
        public int Dest { get => dest; set => dest = value; }
        public onAnswerCmd EventAnswerCmd { get => onAnswerCmd; set => onAnswerCmd = value; }
        public string Resource { get => resource; set => resource = value; }

        internal Cmd(string r,Operation o)
        {
            resource    = r;
            operation   = o;
        }
    }
}
