using System;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using System.Collections.Generic;
using GMap.NET.WindowsForms.Markers;
using System.Drawing;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        LayerMap layerFence, layerRoute, layerBehavior;

        STATUS_GUI statusFence = STATUS_GUI.INIT;
        STATUS_GUI statusTrip = STATUS_GUI.INIT;
        TrackerController trackerController = new TrackerController();

        Fence       fence;
        Route       route;

        int itemselected = -1;

        public MainForm()
        {
            InitializeComponent();

            Protocol.init();

            ThreadManager.init();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            initMapControl();
            initLayers();
            initPanelTrip();
            initPanelFence();
            initPanelBehavior();
            initPanelConfig();

            ThreadManager.startThreads();
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
                fence.clear();
                statusFence = STATUS_GUI.NEW_FENCE;
                button5.Enabled = false;
            }
        }

        //Confirma Fence
        private void btn_fence_Click(object sender, EventArgs e)
        {
            if (statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                add(fence);
                statusFence = STATUS_GUI.CONFIRM_FENCE;
                button5.Enabled = true;
            }
        }

        // Cancel fence
        private void btn_cancel_Click(object sender, EventArgs e)
        {
            statusFence = STATUS_GUI.INIT_OK;
            button5.Enabled = true;
            fence.clear();
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
            removeFenceAt(cbListFence.SelectedIndex);
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
            updateBehavior();
        }

        private void groupBox1_Click(object sender, System.EventArgs e)
        {
            removeRoute(route);

            initSelectRoute();
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
            gMapControl1.Position = new PointLatLng(CAPAO.LATITUDE, CAPAO.LONGITUDE);
        }

        private void initPanelTrip()
        {
            route = trackerController.createRoute();
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
        //-------------------------------------Fim inits -----------------------------------

        private void initSelectRoute()
        {
            txtLatStop.BackColor    = Color.White;
            txtLngStop.BackColor    = Color.White;
            txtLatStart.BackColor   = Color.White;
            txtLngStart.BackColor   = Color.White;

            route.clear();

            statusTrip              = STATUS_GUI.START_POINT;

            txtLatStart.Text        = "";
            txtLngStart.Text        = "";
            txtLatStop.Text         = "";
            txtLngStop.Text         = "";


            txtLatStart.Focus();
        }

        void updateBehavior()
        {
            BuildTreeView bTV = new BuildTreeView(tvBehavior);

            List<Behavior> list = trackerController.getBehaviorFiltered(cbFilter.SelectedIndex);

            bTV.loadlistPointsTreeView(list);
            showMarkerBehavior(list);
        }

        void showMarkerBehavior(List<Behavior> list)
        {
            if (list != null)
            {
                layerBehavior.removeAllMarkers();

                foreach (Behavior b in list)
                {
                    PointLatLng p = new PointLatLng(b.Position.Latitude, b.Position.Longitude);
                    GMarkerGoogleType color = b.OK() ? GMarkerGoogleType.green : GMarkerGoogleType.red;
                    layerBehavior.add(p, b.getStrNOK(), color);
                }
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
            route.startTrip(point);

            txtLatStart.Text = point.Lat.ToString();
            txtLngStart.Text = point.Lng.ToString();

            txtLatStart.BackColor = Color.FromArgb(61, 120, 165);
            txtLngStart.BackColor = Color.FromArgb(61, 120, 165);

            layerRoute.add(point, GMarkerGoogleType.blue);

            txtLatStop.Focus();

            statusTrip = STATUS_GUI.END_POINT;
        }

        private void setEndPoint(PointLatLng point)
        {
            route.stopTrip(point);

            txtLatStop.Text = point.Lat.ToString();
            txtLngStop.Text = point.Lng.ToString();

            txtLatStop.BackColor = Color.FromArgb(61, 120, 165);
            txtLngStop.BackColor = Color.FromArgb(61, 120, 165);

            layerRoute.add(point, GMarkerGoogleType.blue);

            statusTrip = STATUS_GUI.INIT_OK;

            add(route);
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

        internal void add(Fence fence) {

            string str = string.Format("Fence:{0}", trackerController.Fences.Count+1);

            layerFence.add(str, fence);
            trackerController.addFence(fence);
            cbListFence.Items.Add(str);
            cbListFence.SelectedIndex = cbListFence.Items.Count - 1;
        }

        internal void add(Route r)
        {
            route.createRoute();

            trackerController.addRoute(route);
            layerRoute.add(route);
        }

        void removeFenceAt(int index)
        {
            if (index >= 0)
            {
                layerFence.removeFenceAt(index);
                trackerController.removeFenceAt(index);
                cbListFence.Items.RemoveAt(index);
                cbListFence.Text = "";
            }

            initPanelFence(); //clear entitys
        }

        void removePositionFence(int index)
        {
            if (index >= 0)
            {
                fence.removePositionAt(index);
                layerFence.removeMarkerAt(index);
            }
        }

        void addPositionFence(PointLatLng point)
        {
            fence.insertPositon(point);
            layerFence.add(point, GMarkerGoogleType.yellow);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ThreadManager.stopThreads();
        }

        private void panel3_VisibleChanged(object sender, EventArgs e)
        {
            if (panel3.Visible)
            {
                updateBehavior();
            }
        }

        void removeRoute(Route route)
        {
            if (route !=null)
            {
                layerBehavior.removeAllMarkers();
                layerRoute.remove(route);
                trackerController.remove(route);
            }
        }
    }

    class CAPAO
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