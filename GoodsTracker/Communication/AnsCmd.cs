using System;

namespace GoodsTracker
{
    internal class AnsCmd
    {
        Header          header;
        TelemetriaData  telemetria;

        internal TelemetriaData Telemetria { get => telemetria; set => telemetria = value; }
        internal Header Header { get => header; set => header = value; }

        internal AnsCmd()
        {
            header  = new Header();
            telemetria    = null;
        }

        internal AnsCmd(string r,Operation o)
        {
            header              = new Header();
            header.Resource     = r;
            header.Operation    = o;
            telemetria                = null;
        }
    }
}