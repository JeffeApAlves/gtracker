using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        TelemetriaData  telemetriaData;

        bool            statusLock;

        internal TelemetriaData TelemetriaData { get => telemetriaData; set => telemetriaData = value; }
        public bool StatusLock { get => statusLock; set => statusLock = value; }

        internal Tracker(int val)
        {
            statusLock = false;
            address = val;
        }

        public double getLevel()
        {
            return telemetriaData.Level.Val;
        }

        public void requestBehavior(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.RD, RESOURCE.TELEMETRIA);

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
            if(ans.Header.Resource.Equals(RESOURCE.TELEMETRIA))
            {
                updateDataTelemetria(ans);
            }
            else if(ans.Header.Resource.Equals(RESOURCE.LOCK)){
                // Nao fazer nada pq o status da trava esta na telemetria
            }
        }

        void updateDataTelemetria(AnsCmd ans)
        {
            telemetriaData = ans.Telemetria;
        }

        public TelemetriaData getTelemetria()
        {
            return telemetriaData;
        }
    }
}
