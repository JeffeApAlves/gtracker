using System;

/**
 * 
 * Abstracao do comando que sera enviado aos dispositivos
 * 
 * 
 */

namespace GoodsTracker
{
    class AnsCmd
    {
        HeaderFrame header;
        Telemetria  telemetria;

        internal Telemetria Telemetria { get => telemetria; set => telemetria = value; }
        internal HeaderFrame Header { get => header; set => header = value; }

        internal AnsCmd()
        {
            header      = new HeaderFrame();
            telemetria  = null;
        }

        internal AnsCmd(string r,Operation o)
        {
            header              = new HeaderFrame();
            header.Resource     = r;
            header.Operation    = o;
            telemetria          = null;
        }
    }
}