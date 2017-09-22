using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class FrameAMQP : CommunicationFrame
    {

        internal FrameAMQP(HeaderFrame h, PayLoad p)
            :base(h,p)
        {

        }

        internal FrameAMQP()
            : base()
        {

        }
    }
}
