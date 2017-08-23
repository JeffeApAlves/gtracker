using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    interface IDecoderFrame
    {
        bool setValues(out PayLoad payload, Telemetria b);
        bool getValues(out AnsCmd ans, DataFrame frame);
    }
}
