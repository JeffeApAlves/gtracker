using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    interface InterfaceTracker
    {
        void requestBehavior(CallBackAnsCmd ans);
        void lockVehicle(CallBackAnsCmd ans);
        void unLockVehicle(CallBackAnsCmd ans);

        void getLevel();
        Behavior getBehavior();
    }
}
