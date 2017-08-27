using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    interface FrameSerialization
    {
        bool encode(out PayLoad payload, Telemetria b);
        bool decode(out AnsCmd ans, DataFrame frame);
    }
}