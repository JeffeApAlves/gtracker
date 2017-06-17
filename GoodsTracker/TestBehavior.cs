using GMap.NET;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class Tests
    {
        static internal void tstBehavior(List<Behavior> behaviors,Route route)
        {
            Random rnd = new Random();

            if (route != null && route.MapRoute!=null)
            {
                foreach (PointLatLng p in route.MapRoute.Points)
                {
                    Behavior b = new Behavior();

                    b.DateTime = DateTime.Now;

                    b.setPosition(p.Lat, p.Lng);

                    b.setAcceleration(  rnd.Next(0, 100),
                                        rnd.Next(0, 100),
                                        rnd.Next(0, 100));

                    behaviors.Add(b);
                }

            }
        }
    }
}
