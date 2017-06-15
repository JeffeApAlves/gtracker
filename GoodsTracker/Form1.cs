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
    class POSITION_CONST {

        public const double LATITUDE = -23.673326;
        public const double LONGITUDE = -46.775215;
    }

    public partial class MainForm : Form
    {
        LayerMap    layerFence, layerRoute,layoutBehavior;

        Fence       fence;

        int itemselected        = -1;

        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            showPanel(panel1);
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            showPanel(panel2);
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            showPanel(panel3);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            initMapControl();
            initDataGrid();
            printmarker(new PointLatLng(POSITION_CONST.LATITUDE, POSITION_CONST.LONGITUDE));
        }

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point   = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            txtLat.Text         = point.Lat.ToString();
            txtLng.Text         = point.Lng.ToString();

            printmarker(point);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            double lat = Convert.ToDouble(txtLat.Text);
            double lng = Convert.ToDouble(txtLng.Text);

            fence.insertPositon(lat, lng);
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
            gMapControl1.Position       = new PointLatLng(POSITION_CONST.LATITUDE, POSITION_CONST.LONGITUDE);

            layerFence      = new LayerMap(gMapControl1, "Fence");
            layerRoute      = new LayerMap(gMapControl1, "Route");
            layoutBehavior  = new LayerMap(gMapControl1, "Behavior");
        }

        private void initDataGrid()
        {
            fence   = new Fence();

            dataGridView1.DataSource = fence.getDataTable();
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
            printFence(fence);
        }

        private void printFence(Fence fence)
        {
            layerFence.addFence(fence);
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

        private void button4_Click_1(object sender, EventArgs e)
        {
            showPanel(panel4);
        }

        private void printmarker(PointLatLng point)
        {
            layerRoute.addPosition(point);
        }

        void showPanel(Panel p)
        {
            if (p != null)
            {
                bool show = !p.Visible;

                if (show) {

                    panel1.Visible = false;
                    panel2.Visible = false;
                    panel3.Visible = false;
                    panel4.Visible = false;
                }

                p.Visible = show;
            }
        }
    }
}