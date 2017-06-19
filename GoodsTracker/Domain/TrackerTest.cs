using GMap.NET;
using System;

namespace GoodsTracker
{
    class TrackerTest : CommunicationUnit, InterfaceTracker
    {
        int indexBehavior = 0;

        public TrackerTest()
        {
            Address = 1; 
            indexBehavior = 0;
        }

        public void getLevel()
        {
            throw new NotImplementedException();
        }

        public Behavior getBehavior()
        {
            Behavior b = null;

            if (TrackerController.TrackerCtrl.Routes.Count>0 &&
                TrackerController.TrackerCtrl.Routes[0].MapRoute!=null &&
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > indexBehavior)
            {
                b = new Behavior();

                PointLatLng p = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[indexBehavior++];

                Random rnd = new Random();

                b = new Behavior();

                b.DateTime = DateTime.Now;

                b.setPosition(p.Lat, p.Lng);

                b.setAcceleration(rnd.Next(0, 100),
                                rnd.Next(0, 100),
                                rnd.Next(0, 100));

            }

            return b;
        }

        public void lockVehicle(CallBackAnsCmd ans)
        {
//            Cmd c = new Cmd(IdCmd.CMD_LOCK);
//            c.setCallBack(ans);
//            sendCMD(c);
        }

        public void requestBehavior(CallBackAnsCmd ans)
        {
//            Cmd c = new Cmd(IdCmd.CMD_BEHAVIOR);
//            c.setCallBack(ans);
//            sendCMD(c);
        }

        public void unLockVehicle(CallBackAnsCmd ans)
        {
//            Cmd c = new Cmd(IdCmd.CMD_UNLOCK);
//            c.setCallBack(ans);
//            sendCMD(c);
        }
    }
}
