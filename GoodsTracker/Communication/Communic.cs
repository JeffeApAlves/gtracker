using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    interface Communic
    {
        void register(DeviceBase unit);
        void Start();
        void Stop();
        bool receive();
        bool send(AnsCmd ans);
        bool send(Cmd cmd);
    }
}
