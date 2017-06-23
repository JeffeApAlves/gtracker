using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using System.Collections.Generic;

namespace GoodsTracker
{
    class Route
    {
        List<TelemetriaData>  behaviors;

        GDirections         direction;
        GMapRoute           mapRoute;
        GMapPolygon         polygon;

        List<PointLatLng>   points;
        string              name;

        public List<PointLatLng> Points { get => points; set => points = value; }
        public string Name { get => name; set => name = value; }
        public GDirections Direction { get => direction; set => direction = value; }
        public GMapRoute MapRoute { get => mapRoute; set => mapRoute = value; }
        internal List<TelemetriaData> Behaviors { get => behaviors; /*set => behaviors = value;*/ }

        internal Route(string n)
        {
            name        = n;
            points      = new List<PointLatLng>();
            behaviors   = new List<TelemetriaData>();
        }

        internal void startAddress(PointLatLng point)
        {
            points.Add(point);
        }

        internal void stopAddress(PointLatLng point)
        {
            points.Add(point);
        }

        internal void clear()
        {
            points.Clear();
            behaviors.Clear();
        }

        internal bool createRoute(PointLatLng start, PointLatLng stop)
        {
            startAddress(start);
            stopAddress(stop);

            var routedirection = GMapProviders.GoogleMap.GetDirections(out direction, start, stop, false, false, false, false, false);

            if (direction != null)
            {
                mapRoute = new GMapRoute(direction.Route, name);
            }

            polygon = new GMapPolygon(mapRoute.Points, "fence of route:"+name);

            return direction != null;
        }

        internal void createRoute()
        {
            createRoute(points[0], points[points.Count - 1]);
        }

        internal List<TelemetriaData> getBehaviorFiltered(int i)
        {
            List<TelemetriaData> ret = new List<TelemetriaData>(behaviors);

            // i=0 nao filtra nada
            // OK (i=1) entao remove os NOK
            // NOK(i=2) entao remove os OK
            if (i != 0)
            {
                foreach (TelemetriaData b in behaviors)
                {
                    if ((i == 1 && !b.OK()) || i == 2 && b.OK())
                    {
                        ret.Remove(b);
                    }
                }
            }

            return ret;
        }

        internal void registerBehavior(TelemetriaData b)
        {
            if (b != null)
            {
                IsInRoute(b);
                behaviors.Add(b);
            }
        }

        internal string EndAddress()
        {
            return direction.EndAddress;
        }

        internal string StartAddress()
        {
            return direction.StartAddress;
        }

        internal bool IsInRoute(TelemetriaData telemetria)
        {
            bool ret = false;

            if (polygon != null)
            {
                telemetria.InsideOfRoute = polygon.IsInside(new PointLatLng(telemetria.Latitude, telemetria.Longitude));
            }

            return ret;
        }
    }
}
