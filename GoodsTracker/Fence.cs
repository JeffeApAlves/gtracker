using System;
using System.Data;
using System.Drawing;

using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using System.Collections.Generic;

namespace GoodsTracker
{
    class Fence
    {
        private List<PointLatLng> points    = null;
        private GMapPolygon polygon         = null;

        private string name = "";

        public Fence(string pname)
        {
            setName(pname);

            points = new List<PointLatLng>();
        }

        public Fence()
        {
            setName("");

            points      = new List<PointLatLng>();
        }

        public void add(double plat, double plong)
        {
            points.Add(new PointLatLng(plat, plong));
        }

        public void setName(string pname)
        {
            name = pname;
        }

        public void setFence(DataTable pdt)
        {
            double lat, lng;

            for (int i = 0; i < pdt.Rows.Count; i++)
            {
                lat = Convert.ToDouble(pdt.Rows[i]["Latitude"].ToString());
                lng = Convert.ToDouble(pdt.Rows[i]["Longitude"].ToString());
                points.Add(new PointLatLng(lat, lng));
            }

            polygon = new GMapPolygon(points, "mypolygon");
            polygon.Fill = new SolidBrush(Color.FromArgb(50, Color.Red));
            polygon.Stroke = new Pen(Color.Red, 1);
        }

        public GMapPolygon getFence()
        {
            return polygon;
        }
    }
}
