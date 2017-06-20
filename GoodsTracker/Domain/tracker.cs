using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        TelemetriaData    behavior;

        internal Tracker(int val)
        {
            address = val;
        }

        public TelemetriaData getBehavior()
        {
            return behavior;
        }

        public double getLevel()
        {
            return behavior.Level.Val;
        }

        public void requestBehavior(onAnswerCmd on_ans)
        {
            sendCMD(2,Operation.RD,RESOURCE.BEHAVIOR).setEventAnswerCmd(on_ans);
        }

        public void lockVehicle(onAnswerCmd on_ans)
        {
            sendCMD(2, Operation.WR, RESOURCE.LOCK).setEventAnswerCmd(on_ans);
        }

        public void unLockVehicle(onAnswerCmd on_ans)
        {
            sendCMD(2, Operation.WR, RESOURCE.LOCK).setEventAnswerCmd(on_ans);
        }

        /*
         * 
         * Chamado quando um aresposta e recebida 
         * 
         */
        protected override void onReceiveAnswer(AnsCmd ans)
        {
            if(ans.Resource.Equals(RESOURCE.BEHAVIOR))
            {
                updateBehavior(ans);
            }
            else if(ans.Resource.Equals(RESOURCE.LOCK)){
            // NOTHING TODO
            }
        }

        void updateBehavior(AnsCmd ans)
        {
//            behavior = new TelemetriaData();
            behavior = ans.Info;
        }
    }
}
