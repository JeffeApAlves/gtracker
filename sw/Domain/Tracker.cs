﻿using System.Diagnostics;

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
         /*   Cmd cmd = createCMD(Master.ADDRESS, 2, Operation.WR, RESOURCE.LCK);

            statusLock = true;
            cmd.Append("1");
            sendCMD(cmd, on_ans);*/
        }

        public void unLockVehicle(onAnswerCmd on_ans)
        {
            /*Cmd cmd = createCMD(Master.ADDRESS, 2, Operation.WR, RESOURCE.LCK);

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
            else if(ans.Header.Resource.Equals(RESOURCE.LCK)){

                telemetriaData.StatusLock = statusLock;
            }
        }

        void updateDataTelemetria(AnsCmd ans)
        {
            telemetriaData = ans.Telemetria;

        
            Debug.WriteLine("timestamp[Header]:" + ans.Header.TimeStamp);
            Debug.WriteLine("X: {0} : {1}", telemetriaData.AxisX.Val.Val, telemetriaData.AxisX.Val_G.Val);
            Debug.WriteLine("Y: {0} : {1}", telemetriaData.AxisY.Val.Val, telemetriaData.AxisY.Val_G.Val);
            Debug.WriteLine("Z: {0} : {1}", telemetriaData.AxisZ.Val.Val, telemetriaData.AxisZ.Val_G.Val);
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
