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
            publishAnswer();
        }

        static void publishAnswer()
        {
            if (TrackerController.TrackerCtrl.anyRoute() &&
                    TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > count_publish)

            // (/*Communication.isAnyTxCmd() &&*/ !Communication.isAnyAns() /*&& Communication.TxCmds.ContainsKey(RESOURCE.TLM)*/)
            {
                AnsCmd ans = createAnsCmd();
                if (ans != null)
                {
                    Communication.Communic.send(createAnsCmd());
                }
            }
        }

        static AnsCmd createAnsCmd()
        {
            AnsCmd ans = new AnsCmd(RESOURCE.TLM, Operation.AN);

            ans.Header.Address = 2;
            ans.Header.Dest = 1;
            ans.Header.Count = Communication.TxCmds.ContainsKey(RESOURCE.TLM )?Communication.TxCmds[RESOURCE.TLM].Header.Count: count_publish++;
            ans.Telemetria = createTelemetriaData();
            return ans;
        }

        static Telemetria createTelemetriaData()
        {
            Telemetria b = new Telemetria();

            if (TrackerController.TrackerCtrl.anyRoute() &&
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > count_publish)
            {
                Random rnd = new Random();
                PointLatLng p = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[count_publish++];

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
    }
}
