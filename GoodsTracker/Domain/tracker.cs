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

        
            Debug.WriteLine("timestamp[Header]:" + ans.Header.TimeStamp);
            Debug.WriteLine("X: {0} : {1}", telemetriaData.AxisX.Acceleration.Val, telemetriaData.AxisX.Rotation.Val);
            Debug.WriteLine("Y: {0} : {1}", telemetriaData.AxisY.Acceleration.Val, telemetriaData.AxisY.Rotation.Val);
            Debug.WriteLine("Z: {0} : {1}", telemetriaData.AxisZ.Acceleration.Val, telemetriaData.AxisZ.Rotation.Val);
            Debug.WriteLine("Lat: {0} Lng:{1}", telemetriaData.Latitude, telemetriaData.Longitude);
            Debug.WriteLine("data/hora da tlm: "+ ans.Telemetria.DateTime.ToString());
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
