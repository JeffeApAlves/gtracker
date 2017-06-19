using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        public Tracker()
        {
        }

        public Behavior getPosition()
        {
            throw new NotImplementedException();
        }

        public override void update(ObjectValueRX dados)
        {
            throw new NotImplementedException();
        }
    }
}
