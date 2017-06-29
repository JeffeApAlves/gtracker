using System;
using GMap.NET;

namespace GoodsTracker
{
    class TestData : ThreadRun
    {
        static int count_publish = 0;

        public TestData(int t)
        {
            setTime(t);
        }

        public override void run()
        {
            addFrame();
        }

        static void addFrame()
        {
            if (CommunicationUnit.isAnyTxCmd() && !CommunicationUnit.isAnyAns())
            {
                DataTelemetria b            = createTelemetriaData();
                AnsCmd ans                  = createAnsCmd();
                DataFrame frame    = createFrame(b,ans);

                Protocol.Communication.setFrameRx(frame);
            }
        }

        static DataTelemetria createTelemetriaData()
        {
            DataTelemetria b = null;

            if (TrackerController.TrackerCtrl.anyRoute() &&  
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > count_publish)
            {
                b = new DataTelemetria();

                Random rnd = new Random();
                PointLatLng p = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[count_publish++];

                b.setPosition(p.Lat, p.Lng);
                b.setAcceleration(rnd.Next(0, 4), rnd.Next(5, 9), rnd.Next(10, 14));
                b.setRotation(rnd.Next(15,20), rnd.Next(21, 25), rnd.Next(26, 30));
                b.setSpeed(rnd.Next(40, 120));
                b.setLevel(rnd.Next(900, 1000));
                b.Date = DateTime.Now;
                b.Time = DateTime.Now;
            }

            return b;
        }

        static AnsCmd createAnsCmd()
        {
            AnsCmd ans = new AnsCmd(RESOURCE.TLM,Operation.AN);

            ans.Header.Address = 2;
            ans.Header.Dest    = 1;

            return ans;
        }

        static DataFrame createFrame(DataTelemetria b,AnsCmd cmd)
        {
            DataFrame frame = null;

            if (b != null)
            {
                PayLoad payload;

                DecoderFrame decoder = new DecoderFrame();
                decoder.setValues(out payload, b);

                frame = new DataFrame(cmd.Header, payload);
            }

            return frame;
        }
    }
}
