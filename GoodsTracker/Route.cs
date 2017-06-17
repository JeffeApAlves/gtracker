using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using System.Collections.Generic;
using System.Data;


namespace GoodsTracker
{
    class Route
    {
        GDirections direction;
        GMapRoute   mapRoute;

        DataTable dt;
        List<PointLatLng> points;
        string name;

        public DataTable Dt { get => dt; set => dt = value; }
        public List<PointLatLng> Points { get => points; set => points = value; }
        public string Name { get => name; set => name = value; }
        public GDirections Direction { get => direction; set => direction = value; }
        public GMapRoute MapRoute { get => mapRoute; set => mapRoute = value; }

        internal Route(string n)
        {
            name = n;

            points = new List<PointLatLng>();

            dt = new DataTable();

            dt.Columns.Add(new DataColumn("Loc.", typeof(string)));
            dt.Columns.Add(new DataColumn("Latitude", typeof(double)));
            dt.Columns.Add(new DataColumn("Longitude", typeof(double)));
        }

        internal void startTrip(PointLatLng point)
        {
            points.Add(point);
            dt.Rows.Add(string.Format("Start{0}", dt.Rows.Count), point.Lat, point.Lng);
        }

        internal void stopTrip(PointLatLng point)
        {
            points.Add(point);
            dt.Rows.Add(string.Format("Stop{0}", dt.Rows.Count), point.Lat, point.Lng);
        }

        internal void clear()
        {
            points.Clear();
            dt.Clear();
        }

        internal void createRoute(PointLatLng start, PointLatLng stop)
        {
            startTrip(start);

            stopTrip(stop);

            var routedirection = GMapProviders.GoogleMap.GetDirections(out direction, start, stop, false, false, false, false, false);

            mapRoute = new GMapRoute(direction.Route, name);
        }

        internal void createRoute()
        {
            createRoute(points[0], points[points.Count - 1]);
        }
    }
}
