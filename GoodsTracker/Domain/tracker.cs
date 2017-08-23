using System.Diagnostics;

namespace GoodsTracker
{
    class Tracker : BaseCommunication,InterfaceTracker
    {
        DataTelemetria  telemetriaData;
        Stopwatch sw_tlm = new Stopwatch();

        bool statusLock;

        internal DataTelemetria TelemetriaData { get => telemetriaData; set => telemetriaData = value; }
        public bool StatusLock { get => statusLock; set => statusLock = value; }

        internal Tracker(int val):base(val)
        {
            statusLock = false;
            sw_tlm.Start();
        }

        public double getLevel()
        {
            return telemetriaData.Level.Val;
        }

        public void requestBehavior(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.RD, RESOURCE.TLM);
            sendCMD(cmd, on_ans);
        }

        public void lockVehicle(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.WR, RESOURCE.LOCK);

            statusLock = true;
            cmd.Append("1");
            sendCMD(cmd, on_ans);
        }

        public void unLockVehicle(onAnswerCmd on_ans)
        {
            Cmd cmd = createCMD(2, Operation.WR, RESOURCE.LOCK);

            statusLock = false;
            cmd.Append("0");
            sendCMD(cmd, on_ans);
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
                sw_tlm.Restart();
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

        public int getLastUpdate()
        {
            return sw_tlm.Elapsed.Seconds;
        }
    }
}
