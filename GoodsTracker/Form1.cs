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
        LayerMap layerFence, layerRoute;

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

            if (panel1.Visible)
            {
                panel2.Visible = false;
                panel3.Visible = false;
            }
            
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            panel2.Visible = !panel2.Visible;

            if (panel2.Visible)
            {
                panel1.Visible = false;
                panel3.Visible = false;
            }

        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            panel3.Visible = !panel3.Visible;

            if (panel3.Visible)
            {
                panel2.Visible = false;
                panel1.Visible = false;
            }

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
                layerRoute.removePositionAt(itemselected + 1);
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

            layerFence = new LayerMap(gMapControl1, "Fence");
            layerRoute = new LayerMap(gMapControl1, "Route");
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
            printFence(dt);
        }

        private void printFence(DataTable data)
        {
            layerFence.addFence(data);
        }

        private void printFence(Fence fence)
        {
            layerFence.addFence(fence);
        }

        private void printmarker(double lat, double lng)
        {
            layerRoute.addPosition(lat,lng);
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                gMapControl1.MapProvider = GMapProviders.GoogleChinaSatelliteMap;
            }
            else
            {
                gMapControl1.MapProvider = GMapProviders.GoogleMap;
            }
        }

        private void tB_Zoom_Scroll(object sender, EventArgs e)
        {
            gMapControl1.Zoom = tB_Zoom.Value;
        }

        private void flowLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void textBox9_TextChanged(object sender, EventArgs e)
        {

        }

        private void printmarker(PointLatLng point)
        {
            layerRoute.addPosition(point);
        }
    }
}