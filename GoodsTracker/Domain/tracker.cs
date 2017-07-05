using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        DataTelemetria  telemetriaData;

        bool            statusLock;

        internal DataTelemetria TelemetriaData { get => telemetriaData; set => telemetriaData = value; }
        public bool StatusLock { get => statusLock; set => statusLock = value; }

        internal Tracker(int val):base(val)
        {
            statusLock = false;
        }

        public double getLevel()
        {
            return telemetriaData.Level.Val;
        }

        public void requestBehavior(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.RD, RESOURCE.TLM);

            cmd.EventAnswerCmd = on_ans;

            sendCMD(cmd);
        }

        public void lockVehicle(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.WR, RESOURCE.LOCK);

            statusLock = true;
            cmd.Append("1");
            cmd.EventAnswerCmd = on_ans;

            sendCMD(cmd);
        }

        public void unLockVehicle(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.WR, RESOURCE.LOCK);

            statusLock = false;
            cmd.Append("0");
            cmd.EventAnswerCmd = on_ans;

            sendCMD(cmd);
        }

        /*
         * 
         * Hook para processamento de comandos respondidos
         * 
         */
        protected override void onReceiveAnswer(AnsCmd ans)
        {
            if(ans.Header.Resource.Equals(RESOURCE.TLM))
            {
                updateDataTelemetria(ans);
            }
            else if(ans.Header.Resource.Equals(RESOURCE.LOCK)){

                telemetriaData.StatusLock = statusLock;
            }
        }

        void updateDataTelemetria(AnsCmd ans)
        {
            telemetriaData = ans.Telemetria;
        }

        public DataTelemetria getTelemetria()
        {
            return telemetriaData;
        }
    }
}
