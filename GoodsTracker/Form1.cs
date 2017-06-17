using System;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using System.Collections.Generic;
using GMap.NET.WindowsForms.Markers;
using System.Drawing;
using GMap.NET.WindowsForms;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        LayerMap layerFence, layerRoute, layerBehavior;

        STATUS_GUI statusFence = STATUS_GUI.INIT;
        STATUS_GUI statusTrip = STATUS_GUI.INIT;
        TrackerController trackerController = new TrackerController();

        Fence fence;
        int itemselected = -1;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            initLayers();
            initMapControl();
            initPanelFence();
            initPanelConfig();
            initPanelBehavior();

//            layerRoute.addPosition(gMapControl1.Position, GMarkerGoogleType.blue);
        }

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            if (statusTrip.Equals(STATUS_GUI.START_POINT))
            {
                setStartPoint(point);
            }
            else if (statusTrip.Equals(STATUS_GUI.END_POINT))
            {
                setEndPoint(point);
            }
            else if (statusFence.Equals(STATUS_GUI.NEW_FENCE) || statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                setFencePoint(point);
            }
        }

        //Seleciona painel trip
        private void button1_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel1);
        }

        //Seleciona painel Fence
        private void button2_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel2);
        }

        //Seleciona painel Behavior
        private void button3_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel3);
        }

        // seleciona painel configuracao
        private void button4_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel4);
        }

        //Inicia nova fence
        private void button5_Click(object sender, EventArgs e)
        {
            if (!statusFence.Equals(STATUS_GUI.NEW_FENCE))
            {
                statusFence = STATUS_GUI.NEW_FENCE;

                fence.clearPoints();

                button5.Enabled = false;
            }
        }

        //Confirma Fence
        private void btn_fence_Click(object sender, EventArgs e)
        {
            if (statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                addFence(fence);

                statusFence = STATUS_GUI.CONFIRM_FENCE;

                button5.Enabled = true;
            }
        }

        // Cancel fence
        private void btn_cancel_Click(object sender, EventArgs e)
        {
            statusFence = STATUS_GUI.INIT_OK;

            button5.Enabled = true;

            fence.clearPoints();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                removePositionFence(itemselected);
            }
        }

        // Remove Fence selecionada no combo box
        private void btn_delFence_Click(object sender, EventArgs e)
        {
            removeFence(cbListFence.SelectedIndex);
        }

        // Seleciona ponto no gridview
        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            itemselected = e.RowIndex;

            if (itemselected >= 0) {

                txtLat.Text = dataGridView1.Rows[itemselected].Cells[1].Value.ToString();
                txtLng.Text = dataGridView1.Rows[itemselected].Cells[2].Value.ToString();
            }
        }

        //Atualiza zoom do mapa conforme track
        private void tB_Zoom_Scroll(object sender, EventArgs e)
        {
            gMapControl1.Zoom = tB_Zoom.Value;
        }

        //Seleciona layers
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

        private void cbFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            showListBehavior();
        }

        private void groupBox1_Click(object sender, System.EventArgs e)
        {
            txtLatStart.Focus();
            statusTrip = STATUS_GUI.START_POINT;
        }

        private void groupBox2_Click(object sender, System.EventArgs e)
        {
            statusTrip = STATUS_GUI.END_POINT;
        }

        private void txtLatStart_Enter(object sender, EventArgs e)
        {
            txtLatStart.BackColor = Color.Yellow;
            txtLngStart.BackColor = Color.Yellow;
        }

        private void txtLatStop_Enter(object sender, EventArgs e)
        {
            txtLatStop.BackColor = Color.White;
            txtLngStop.BackColor = Color.White;

            txtLatStop.BackColor = Color.Yellow;
            txtLngStop.BackColor = Color.Yellow;
        }
        //---------------------------------End Events-----------------------------------

        private void initMapControl()
        {
            gMapControl1.DragButton = MouseButtons.Left;
            gMapControl1.CanDragMap = true;
            gMapControl1.MapProvider = GMapProviders.GoogleMap;
            gMapControl1.MinZoom = 0;
            gMapControl1.MaxZoom = 24;
            gMapControl1.Zoom = 15;
            gMapControl1.AutoScroll = true;
            gMapControl1.Position = new PointLatLng(POSITION_CONST.LATITUDE, POSITION_CONST.LONGITUDE);
        }

        private void initPanelFence()
        {
            fence = trackerController.createFence();

            dataGridView1.DataSource = fence.Data;

            txtLat.Text = "";
            txtLng.Text = "";
        }

        private void initLayers()
        {
            layerFence = new LayerMap(gMapControl1, "Fence");
            layerRoute = new LayerMap(gMapControl1, "Route");
            layerBehavior = new LayerMap(gMapControl1, "Behavior");
        }

        void initPanelConfig()
        {
            checkedListBox1.SetItemCheckState(0, layerRoute.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(1, layerBehavior.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(2, layerFence.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(3, gMapControl1.MapProvider.Equals(GMapProviders.GoogleChinaSatelliteMap) ? CheckState.Checked : CheckState.Unchecked);
        }

        void initPanelBehavior()
        {
            cbFilter.SelectedIndex = 0;
        }

        void showListBehavior()
        {
            BuildTreeView bTV = new BuildTreeView(tvBehavior);

            List<Behavior> list = trackerController.getBehaviorFiltered(cbFilter.SelectedIndex);

            bTV.loadlistPointsTreeView(list);

            showMarkerBehavior(list);
        }

        void showMarkerBehavior(List<Behavior> list)
        {
            layerBehavior.clearPositions();

            foreach (Behavior b in list)
            {
                PointLatLng p = new PointLatLng(b.Position.Latitude, b.Position.Longitude);

                GMarkerGoogleType color = b.OK() ? GMarkerGoogleType.green : GMarkerGoogleType.red;

                layerBehavior.addPosition(p, b.getStrNOK(), color);
            }
        }

        private void setFencePoint(PointLatLng point)
        {
            txtLat.Text = point.Lat.ToString();
            txtLng.Text = point.Lng.ToString();

            addPositionFence(point);
            statusFence = STATUS_GUI.ADD_POINTS;
            btn_fence.Enabled = true;
        }

        private void setStartPoint(PointLatLng point)
        {
            txtLatStart.Text = point.Lat.ToString();
            txtLngStart.Text = point.Lng.ToString();

            txtLatStart.BackColor = Color.FromArgb(61, 120, 165);
            txtLngStart.BackColor = Color.FromArgb(61, 120, 165);

            layerRoute.addPosition(point, GMarkerGoogleType.blue);

            txtLatStop.Focus();

            statusTrip = STATUS_GUI.END_POINT;
        }

        private void setEndPoint(PointLatLng point)
        {
            txtLatStop.Text = point.Lat.ToString();
            txtLngStop.Text = point.Lng.ToString();

            txtLatStop.BackColor = Color.FromArgb(61, 120, 165);
            txtLngStop.BackColor = Color.FromArgb(61, 120, 165);

            layerRoute.addPosition(point, GMarkerGoogleType.blue);

            statusTrip = STATUS_GUI.INIT_OK;

            createRoute(layerRoute.getStartPosition(), layerRoute.getEndPosition());
        }

        //seleciona painel
        void selectPanel(Panel p)
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

        void addFence(Fence fence) {

            string str = string.Format("Fence:{0}", trackerController.ListFence.Count+1);

            layerFence.addFence(str, fence);
            trackerController.addFence(fence);
            cbListFence.Items.Add(str);
            cbListFence.SelectedIndex = cbListFence.Items.Count - 1;
        }

        void removeFence(int index)
        {
            if (index >= 0)
            {
                layerFence.removeFenceAt(index);
                trackerController.removeFenceAt(index);
                cbListFence.Items.RemoveAt(index);
                cbListFence.Text = "";

                initPanelFence(); //clear DataSource
            }
        }

        void removePositionFence(int index)
        {
            if (index >= 0)
            {
                fence.removePositionAt(index);
                layerFence.removePositionAt(index);
            }
        }

        void addPositionFence(PointLatLng point)
        {
            fence.insertPositon(point);
            layerFence.addPosition(point, GMarkerGoogleType.yellow);
        }

        internal void createRoute(PointLatLng start, PointLatLng stop)
        {
            GDirections direction;

            var routedirection  = GMapProviders.GoogleMap.GetDirections(out direction, start, stop, false, false, false, false,false);
            GMapRoute route     = new GMapRoute(direction.Route, "Rota");

            layerRoute.addRoute(route);

            gMapControl1.Refresh();
        }
    }

    class POSITION_CONST
    {
        internal const double LATITUDE  = -23.673326;
        internal const double LONGITUDE = -46.775215;
    }

    enum STATUS_GUI {

        INIT,
        INIT_OK,
        NEW_FENCE,
        ADD_POINTS,
        CONFIRM_FENCE,
        START_POINT,
        END_POINT
    }
}