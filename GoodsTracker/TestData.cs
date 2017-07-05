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
            start();
        }

        public override void run()
        {
            publishAnswer();
        }

        static void publishAnswer()
        {
            if (Communication.isAnyTxCmd() && !Communication.isAnyAns() && Communication.TxCmds.ContainsKey(RESOURCE.TLM))
            {
                DataFrame frame = createFrame(  createTelemetriaData(), 
                                                createAnsCmd());
                if (frame != null)
                {
                    Communication.publishAnswer(frame);
                }
            }
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

        static DataTelemetria createTelemetriaData()
        {
            DataTelemetria b = null;

            if (TrackerController.TrackerCtrl.anyRoute() &&
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > count_publish)
            {
                b = new DataTelemetria();

                Random rnd      = new Random();
                PointLatLng p   = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[count_publish++];

                b.setPosition(p.Lat * 100, p.Lng * 100);
                b.setAcceleration(rnd.Next(0, 4), rnd.Next(5, 9), rnd.Next(10, 14));
                b.setRotation(rnd.Next(15, 20), rnd.Next(21, 25), rnd.Next(26, 30));
                b.setSpeed(rnd.Next(40, 120));
                b.setLevel(rnd.Next(10000, 50000));
                b.Date = DateTime.Now;
                b.Time = DateTime.Now;
            }

            return b;
        }

        static AnsCmd createAnsCmd()
        {
            AnsCmd ans = new AnsCmd(RESOURCE.TLM, Operation.AN);

            ans.Header.Address = 2;
            ans.Header.Dest = 1;
            ans.Header.Count = Communication.TxCmds[RESOURCE.TLM].Header.Count;

            return ans;
        }
    }
}
