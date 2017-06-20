using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    interface InterfaceTracker
    {
        void requestBehavior(onAnswerCmd ans);
        void lockVehicle(onAnswerCmd ans);
        void unLockVehicle(onAnswerCmd ans);
        double  getLevel();
        Behavior getBehavior();
    }
}
