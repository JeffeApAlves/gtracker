using GMap.NET;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class TrackerTest : CommunicationUnit, InterfaceTracker
    {
        int indexBehavior=0;

        public TrackerTest()
        {
            indexBehavior = 0;
        }

        public Behavior getPosition()
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

        public override void update(ObjectValueRX dados)
        {
            throw new NotImplementedException();
        }
    }
}
