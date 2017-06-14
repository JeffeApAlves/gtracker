using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System.Collections;
using System.Collections.Generic;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        GMarkerGoogle marker;
        GMapOverlay markerOverlay;
        DataTable dt;

        double latitude = -23.673326;
        double longitude = -46.775215;

        public MainForm()
        {
            InitializeComponent();
        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            panel1.Visible = !panel1.Visible;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            panel2.Visible = !panel2.Visible;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            panel3.Visible = !panel3.Visible;
        }

        private void panel3_Paint(object sender, PaintEventArgs e)
        {

        }

        private void gMapControl1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            panel1.Visible = !panel1.Visible;
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            panel2.Visible = !panel2.Visible;
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            panel3.Visible = !panel3.Visible;
        }

        private void button4_Click(object sender, EventArgs e)
        {

        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            gMapControl1.DragButton = MouseButtons.Left;
            gMapControl1.CanDragMap = true;
            gMapControl1.MapProvider = GMapProviders.GoogleMap;
            gMapControl1.Position = new PointLatLng(latitude, longitude);
            gMapControl1.MinZoom = 0;
            gMapControl1.MaxZoom = 24;
            gMapControl1.Zoom = 22;
            gMapControl1.AutoScroll = true;

            markerOverlay = new GMapOverlay("Marcador");
            marker = new GMarkerGoogle(new PointLatLng(latitude, longitude),GMarkerGoogleType.green);
            markerOverlay.Markers.Add(marker);

            marker.ToolTipMode = MarkerTooltipMode.Always;
            marker.ToolTipText = string.Format("Localizacao\n Latitude:{0} \n Longitude:{1}", latitude, longitude);

            gMapControl1.Overlays.Add(markerOverlay);


  
            GMapOverlay polyOverlay = new GMapOverlay("polygons");
            List<PointLatLng> points = new List<PointLatLng>();
            points.Add(new PointLatLng(latitude - 0.1, longitude + 0.1));
            points.Add(new PointLatLng(latitude - 0.1, longitude + 0.1));
            points.Add(new PointLatLng(latitude + 0.1, longitude - 0.1));
            points.Add(new PointLatLng(latitude + 0.1, longitude - 0.1));
            GMapPolygon polygon = new GMapPolygon(points, "mypolygon");
            polygon.Fill = new SolidBrush(Color.FromArgb(50, Color.Red));
            polygon.Stroke = new Pen(Color.Red, 1);
            polyOverlay.Polygons.Add(polygon);
            gMapControl1.Overlays.Add(polyOverlay);
        }
    }
}
