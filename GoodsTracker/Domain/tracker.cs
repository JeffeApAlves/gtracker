using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        TelemetriaData    telemetriaData;

        internal TelemetriaData TelemetriaData { get => telemetriaData; set => telemetriaData = value; }

        internal Tracker(int val)
        {
            address = val;
        }

        public double getLevel()
        {
            return telemetriaData.Level.Val;
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
                updateDataTelemetria(ans);
            }
            else if(ans.Resource.Equals(RESOURCE.LOCK)){
                // NOTHING UP To NOW
            }
        }

        void updateDataTelemetria(AnsCmd ans)
        {
            telemetriaData = ans.Info;
        }

        public TelemetriaData getTelemetria()
        {
            return telemetriaData;
        }
    }
}
