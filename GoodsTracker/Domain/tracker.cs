using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        public Behavior getBehavior()
        {
            throw new NotImplementedException();
        }

        public void getLevel()
        {
            throw new NotImplementedException();
        }

        public void lockVehicle(CallBackAnsCmd ans)
        {
            throw new NotImplementedException();
        }

        public void requestBehavior(CallBackAnsCmd ans)
        {
            throw new NotImplementedException();
        }

        public void unLockVehicle(CallBackAnsCmd ans)
        {
            throw new NotImplementedException();
        }
    }
}
