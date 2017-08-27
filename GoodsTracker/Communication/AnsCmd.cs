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
        Header          header;
        Telemetria  telemetria;

        internal Telemetria Telemetria { get => telemetria; set => telemetria = value; }
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
            telemetria          = null;
        }
    }
}