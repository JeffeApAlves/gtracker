using GMap.NET;
using System;

namespace GoodsTracker
{
    class TestData : ThreadRun
    {
        static int count_ans        = 0;
        static int count_behavior   = 0;

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
            if (CommunicationUnit.QueueCmd.Count > 0 && CommunicationUnit.QueueAnsCmd.Count<=0)
            {
                count_ans++;

                TelemetriaData b            = createTelemetriaData();
                Cmd cmd                     = createCmd();
                CommunicationFrame frame    = createFrame(b,cmd);

                if (frame != null)
                {
                    Protocol.Communication.setFrameRx(frame.strOfFrame());
                }
            }
        }

        static TelemetriaData createTelemetriaData()
        {
            TelemetriaData b = null;

            if (TrackerController.TrackerCtrl.anyRoute() &&  
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > count_behavior)
            {
                b = new TelemetriaData();

                Random rnd = new Random();
                PointLatLng p = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[count_behavior++];

                b.DateTime = DateTime.Now;
                b.setPosition(p.Lat, p.Lng);
                b.setAcceleration(rnd.Next(0, 4), rnd.Next(5, 9), rnd.Next(10, 14));
                b.setSpeed(rnd.Next(40, 120));
                b.setLevel(rnd.Next(900, 1000));
            }

            return b;
        }

        static Cmd createCmd()
        {
            Cmd cmd = new Cmd(RESOURCE.TELEMETRIA);

            cmd.Operation = Operation.AN;
            cmd.Dest = 1;

            return cmd;
        }

        static CommunicationFrame createFrame(TelemetriaData b,Cmd cmd)
        {
            CommunicationFrame frame = null;

            if (b != null)
            {
                frame = new CommunicationFrame();
                DecoderFrame decoder = new DecoderFrame();

                decoder.setHeader(ref frame, TrackerController.TrackerCtrl.Tracker, cmd);
                decoder.setPayLoad(ref frame, b);
            }

            return frame;
        }
    }
}
