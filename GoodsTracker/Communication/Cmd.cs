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

        Header header;
        
        public onAnswerCmd EventAnswerCmd { get => onAnswerCmd; set => onAnswerCmd = value; }
        internal Header Header { get => header; set => header = value; }

        internal Cmd(string r,Operation o)
        {
            header = new Header();

            header.Resource = r;
            header.Operation = o;
        }
    }
}
