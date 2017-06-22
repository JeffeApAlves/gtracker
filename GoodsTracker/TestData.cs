using GMap.NET;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class TestData : ThreadRun
    {
        public override void run()
        {
            AddFrame();
        }

        static int indexBehavior = 0;

        public TestData(int t)
        {
            setTime(t);
        }

        static public void AddFrame()
        {

            if (TrackerController.TrackerCtrl.anyRoute() &&
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > indexBehavior)
            {
                Random rnd = new Random();
                TelemetriaData b = new TelemetriaData();
                PointLatLng p = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[indexBehavior++];

                b = new TelemetriaData();
                b.DateTime = DateTime.Now;
                b.setPosition(p.Lat, p.Lng);
                b.setAcceleration(rnd.Next(0, 4), rnd.Next(5, 9), rnd.Next(10, 14));
                b.setSpeed(rnd.Next(40, 120));
                b.setLevel(rnd.Next(900, 1000));

                DecoderFrame decoder = new DecoderFrame();
                CommunicationFrame frame = new CommunicationFrame();
                Cmd cmd = new Cmd(RESOURCE.TELEMETRIA);
                //                Tracker t                   = new Tracker(2);
                cmd.Operation = Operation.AN;
                cmd.Dest = 1;

                decoder.setHeader(ref frame, TrackerController.TrackerCtrl.Tracker, cmd);
                decoder.setPayLoad(ref frame, b);

                Protocol.Communication.setFrameRx(CONST_CHAR.RX_FRAME_START +
                                                    frame.Frame +
                                                    CONST_CHAR.SEPARATOR +
                                                    //                                                    CONST_CHAR.ASTERISCO + 
                                                    frame.checkSum().ToString() +
                                                    CONST_CHAR.RX_FRAME_END +
                                                    CONST_CHAR.CR +
                                                    CONST_CHAR.LF);
            }
        }
    }
}
