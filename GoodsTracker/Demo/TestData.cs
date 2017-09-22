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
            Communic communic = GTracker.Communic;

            if (communic!=null && TrackerController.TrackerCtrl.anyRoute() &&
                    TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > count_publish)

            if(Communication.isAnyTxCmd() && !Communication.isAnyAns() && Communication.TxCmds.ContainsKey(RESOURCE.TLM))
            {
                AnsCmd ans = createAnsCmd();
                if (ans != null)
                {
                    communic.send(createAnsCmd());
                }
            }
        }

        static AnsCmd createAnsCmd()
        {
            AnsCmd ans = new AnsCmd(RESOURCE.TLM, Operation.AN);

            ans.Header.Address      = 2;
            ans.Header.Dest         = 1;
            ans.Header.TimeStamp    = (Int32)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;
            ans.Telemetria          = createTelemetriaData();
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

                b.setPosition(AsGrauSexagesimal(p.Lat) * 100, AsGrauSexagesimal(p.Lng) * 100);
                b.setXYZ(rnd.Next(0, 4), rnd.Next(0,4095), rnd.Next(0, 4095));
                b.setXYZ_G(rnd.Next(-120, 120)/100.0, rnd.Next(-120, 120)/100.0, rnd.Next(-120, 120)/100.0);
                b.setSpeed(rnd.Next(40, 120));
                b.setLevel(rnd.Next(5000, 32000));
                b.DateTime = DateTime.Now;
            }

            return b;
        }

        static private double AsGrauSexagesimal(double dest)
        {
            double minutos = dest % 1;
            double graus = dest - minutos;
            double dec = (minutos *0.60);

            return Math.Round(graus + dec, 4);
        }
    }
}
