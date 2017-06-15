using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System.Collections.Generic;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        GMapOverlay     markerOverlay   = null;
        GMapOverlay     polyOverlay     = null;
        DataTable       dt;

        const double LATITUDE   = -23.673326;
        const double LONGITUDE  = -46.775215;

        int itemselected        = -1;

        public MainForm()
        {
            InitializeComponent();
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
            initMapControl();
            initDataTable();
            initDataGrid();
            printmarker(LATITUDE, LONGITUDE);
        }

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            txtLat.Text = point.Lat.ToString();
            txtLng.Text = point.Lng.ToString();

            printmarker(point);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            double lat = Convert.ToDouble(txtLat.Text);
            double lng = Convert.ToDouble(txtLng.Text);

            insertData(lat,lng);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (itemselected >= 0)
            {
                dataGridView1.Rows.RemoveAt(itemselected);
                markerOverlay.Markers.RemoveAt(itemselected+1);
            }            
        }

        private void initMapControl()
        {
            gMapControl1.DragButton     = MouseButtons.Left;
            gMapControl1.CanDragMap     = true;
            gMapControl1.MapProvider    = GMapProviders.GoogleMap;
            gMapControl1.MinZoom        = 0;
            gMapControl1.MaxZoom        = 24;
            gMapControl1.Zoom           = 15;
            gMapControl1.AutoScroll     = true;
            gMapControl1.Position       = new PointLatLng(LATITUDE, LONGITUDE);
        }

        private void initDataTable()
        {
            dt = new DataTable();
            dt.Columns.Add(new DataColumn("Loc.",       typeof(string)));
            dt.Columns.Add(new DataColumn("Latitude",   typeof(double)));
            dt.Columns.Add(new DataColumn("Longitude",  typeof(double)));
        }

        private void initDataGrid()
        {
            dataGridView1.DataSource = dt;
        }

        private void insertData(double lat, double lng)
        {
            dt.Rows.Add(string.Format("{0}", dt.Rows.Count), lat, lng);
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            itemselected    = e.RowIndex;
            //latitude      = dataGridView1.Rows[itemselected].Cells[0].Value.ToString();
            txtLat.Text     = dataGridView1.Rows[itemselected].Cells[1].Value.ToString();
            txtLng.Text     = dataGridView1.Rows[itemselected].Cells[2].Value.ToString();
        }

        private void btn_fence_Click(object sender, EventArgs e)
        {
            Fence fence = new Fence();

            fence.setFence(dt);

            printFence(fence);
        }

        private void printFence(Fence pfence)
        {
            if (polyOverlay == null)
            {
                polyOverlay = new GMapOverlay("Fence");
                gMapControl1.Overlays.Add(polyOverlay);
            }

            polyOverlay.Polygons.Add(pfence.getFence());
        }

        private void printmarker(double lat, double lng)
        {
            GMarkerGoogle marker;

            if (markerOverlay == null)
            {
                markerOverlay = new GMapOverlay("Marcador");
                gMapControl1.Overlays.Add(markerOverlay);
            }


            marker = new GMarkerGoogle(new PointLatLng(lat, lng), GMarkerGoogleType.green);
            marker.ToolTipMode = MarkerTooltipMode.Always;
            marker.ToolTipText = string.Format("Localizacao\n Latitude:{0} \n Longitude:{1}", lat, lng);


            markerOverlay.Markers.Add(marker);
        }

        private void printmarker(PointLatLng point)
        {
            double lat = point.Lat;
            double lng = point.Lng;

            printmarker(lat, lng);
        }
    }
}
