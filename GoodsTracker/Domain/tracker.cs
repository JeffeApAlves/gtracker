using System.Diagnostics;

namespace GoodsTracker
{
    class Tracker : DeviceBase,InterfaceTracker
    {
        Telemetria  telemetriaData;
        Stopwatch sw_tlm = new Stopwatch();

        bool statusLock;

        internal Telemetria TelemetriaData { get => telemetriaData; set => telemetriaData = value; }
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
            Cmd cmd = createCMD(Master.ADDRESS,2, Operation.RD, RESOURCE.TLM);
            sendCMD(cmd, on_ans);
        }

        public void lockVehicle(onAnswerCmd on_ans)
        {
            /*
            Cmd cmd = createCMD(Master.ADDRESS, 2, Operation.WR, RESOURCE.LOCK);

            statusLock = true;
            cmd.Append("1");
            sendCMD(cmd, on_ans);*/
        }

        public void unLockVehicle(onAnswerCmd on_ans)
        {
            /*
            Cmd cmd = createCMD(Master.ADDRESS, 2, Operation.WR, RESOURCE.LOCK);

            statusLock = false;
            cmd.Append("0");
            sendCMD(cmd, on_ans);*/
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
                Debug.WriteLine("ans:" + ans.Header.Count);
                Debug.WriteLine("X: {0} : {1}",ans.Telemetria.AxisX.Acceleration.Val , ans.Telemetria.AxisX.Rotation.Val);
                Debug.WriteLine("Y: {0} : {1}",ans.Telemetria.AxisY.Acceleration.Val, ans.Telemetria.AxisY.Rotation.Val);
                Debug.WriteLine("Z: {0} : {1}",ans.Telemetria.AxisZ.Acceleration.Val, ans.Telemetria.AxisZ.Rotation.Val);

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

        public Telemetria getTelemetria()
        {
            return telemetriaData;
        }

        public int getLastUpdate()
        {
            return (1000*sw_tlm.Elapsed.Seconds)+sw_tlm.Elapsed.Milliseconds;
        }

        public void publishTLM(Telemetria tlm)
        {
            AnsCmd ans = createAnsCmd(2,Master.ADDRESS, RESOURCE.TLM);
        }
    }
}
