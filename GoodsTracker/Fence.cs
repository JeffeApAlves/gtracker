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
        private DataTable           dt;
        private List<PointLatLng>   points;
        private GMapPolygon         polygon;

        private string name = "";

        public Fence(string pname)
        {
            setName(pname);

            points  = new List<PointLatLng>();

            dt      = new DataTable();

            dt.Columns.Add(new DataColumn("Loc.", typeof(string)));
            dt.Columns.Add(new DataColumn("Latitude", typeof(double)));
            dt.Columns.Add(new DataColumn("Longitude", typeof(double)));
        }

        public Fence()
        {
            setName("");

            points = new List<PointLatLng>();

            dt = new DataTable();

            dt.Columns.Add(new DataColumn("Loc.", typeof(string)));
            dt.Columns.Add(new DataColumn("Latitude", typeof(double)));
            dt.Columns.Add(new DataColumn("Longitude", typeof(double)));
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
            dt = pdt;

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

        public DataTable getDataTable()
        {
            return dt;
        }

        public void insertPositon(double lat, double lng)
        {
            dt.Rows.Add(string.Format("{0}", dt.Rows.Count), lat, lng);

            points.Add(new PointLatLng(lat, lng));

            updatePolygon();
        }

        public void insertPositon(PointLatLng point)
        {
            dt.Rows.Add(string.Format("{0}", dt.Rows.Count), point.Lat, point.Lng);

            points.Add(point);

            updatePolygon();
        }

        private void updatePolygon()
        {
            polygon         = new GMapPolygon(points, "mypolygon");
            polygon.Fill    = new SolidBrush(Color.FromArgb(50, Color.Red));
            polygon.Stroke  = new Pen(Color.Red, 1);
        }

        internal void clearPoints()
        {
            dt.Clear();

            points.Clear();
        }

        public void removePositionAt(int index)
        {
            if (index < dt.Rows.Count)
            {
                dt.Rows.RemoveAt(index);

                points.RemoveAt(index);

                updatePolygon();
            }
        }
    }
}
