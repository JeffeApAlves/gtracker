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
        LayerMap layerFence, layerRoute, layerBehavior;

        STATUS_GUI statusGUI    = STATUS_GUI.INIT;

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
            initLayers();
            initPanelConfig();

            insertPointTreeView();

            printmarker(gMapControl1.Position);
        }

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point   = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            txtLat.Text         = point.Lat.ToString();
            txtLng.Text         = point.Lng.ToString();

            printmarker(point);

            if (statusGUI.Equals(STATUS_GUI.NEW_FENCE) || statusGUI.Equals(STATUS_GUI.ADD_POINTS))
            {
                fence.insertPositon(point);

                statusGUI = STATUS_GUI.ADD_POINTS;

                btn_fence.Enabled = true;
            }
        }

        //Inicia nova fence
        private void button5_Click(object sender, EventArgs e)
        {
            if (!statusGUI.Equals(STATUS_GUI.NEW_FENCE))
            {
                statusGUI = STATUS_GUI.NEW_FENCE;

                fence.clearPoints();

                button5.Enabled = false;
            }
        }

        //Confirma Fence
        private void btn_fence_Click(object sender, EventArgs e)
        {
            if (statusGUI.Equals(STATUS_GUI.ADD_POINTS))
            {
                printFence(fence);

                statusGUI = STATUS_GUI.CONFIRM_FENCE;

                button5.Enabled = true;
            }
        }

        // Cancel fence
        private void btn_cancel_Click(object sender, EventArgs e)
        {
            statusGUI = STATUS_GUI.INIT_OK;

            button5.Enabled = true;

            fence.clearPoints();
        }


        private void button6_Click(object sender, EventArgs e)
        {
            if (itemselected >= 0)
            {
                layerRoute.removePositionAt(itemselected + 1);

                fence.removePositionAt(itemselected);
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
        }

        private void initDataGrid()
        {
            fence   = new Fence();

            dataGridView1.DataSource = fence.getDataTable();
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            itemselected    = e.RowIndex;

            if (itemselected >= 0) {

                //latitude      = dataGridView1.Rows[itemselected].Cells[0].Value.ToString();
                txtLat.Text = dataGridView1.Rows[itemselected].Cells[1].Value.ToString();
                txtLng.Text = dataGridView1.Rows[itemselected].Cells[2].Value.ToString();
            }
        }

        private void initLayers()
        {
            layerFence      = new LayerMap(gMapControl1, "Fence");
            layerRoute      = new LayerMap(gMapControl1, "Route");
            layerBehavior   = new LayerMap(gMapControl1, "Behavior");
        }

        private void printFence(Fence fence)
        {
            layerFence.addFence(fence);
        }

        private void tB_Zoom_Scroll(object sender, EventArgs e)
        {
            gMapControl1.Zoom = tB_Zoom.Value;
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            showPanel(panel4);
        }

        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (checkedListBox1.GetItemCheckState(3) == CheckState.Checked)
            {
                gMapControl1.MapProvider = GMapProviders.GoogleChinaSatelliteMap;
            }
            else
            {
                gMapControl1.MapProvider = GMapProviders.GoogleMap;
            }

            layerRoute.show(checkedListBox1.GetItemCheckState(0) == CheckState.Checked);
            layerBehavior.show(checkedListBox1.GetItemCheckState(1) == CheckState.Checked);
            layerFence.show(checkedListBox1.GetItemCheckState(2) == CheckState.Checked);
        }

        void initPanelConfig()
        {
            checkedListBox1.SetItemCheckState(0, layerRoute.isVisible()?CheckState.Checked:CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(1, layerBehavior.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(2, layerFence.isVisible() ? CheckState.Checked : CheckState.Unchecked);
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

        void insertPointTreeView()
        {
            TreeNode root = tvBehavior.Nodes.Add("Trip");

            TreeNode info = root.Nodes.Add("Localizacao:(0.0000 /  0.0000)");

            info.Nodes.Add("Eixo X: Aceleracao 0.00 Rotacao: 0.00");
            info.Nodes.Add("Eixo Y: Aceleracao 0.00 Rotacao: 0.00");
            info.Nodes.Add("Eixo Z: Aceleracao 0.00 Rotacao: 0.00");

            info = root.Nodes.Add("Localizacao:(0.0000 /  0.0000)");

            info.Nodes.Add("Eixo X: Aceleracao 0.00 Rotacao: 0.00");
            info.Nodes.Add("Eixo Y: Aceleracao 0.00 Rotacao: 0.00");
            info.Nodes.Add("Eixo Z: Aceleracao 0.00 Rotacao: 0.00");
        }
    }

    class POSITION_CONST
    {
        public const double LATITUDE = -23.673326;
        public const double LONGITUDE = -46.775215;
    }


    enum STATUS_GUI {

        INIT,
        INIT_OK,
        NEW_FENCE,
        ADD_POINTS,
        CONFIRM_FENCE
    }
}