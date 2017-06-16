using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using System.Collections.Generic;
using GMap.NET.WindowsForms.Markers;
using System.Text;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        LayerMap layerFence, layerRoute, layerBehavior;

        STATUS_GUI  statusFence     = STATUS_GUI.INIT;
        TrackerController trackerController = new TrackerController();

        Fence fence;
        int itemselected        = -1;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            initMapControl();
            initDataGrid();
            initLayers();
            initPanelConfig();
            initPanelBehavior();

            layerRoute.addPosition(gMapControl1.Position);
        }

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

        private void initDataGrid()
        {
            fence = trackerController.createFence();

            dataGridView1.DataSource = fence.Data;
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

            List<Behavior> list = getBehaviorFiltered(cbFilter.SelectedIndex);

            loadlistPointsTreeView(list);

            showMarkerBehavior(list);
        }
        //------------------------------------------------------------------------------------

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point   = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            txtLat.Text         = point.Lat.ToString();
            txtLng.Text         = point.Lng.ToString();

            if (statusFence.Equals(STATUS_GUI.NEW_FENCE) || statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                addPositionFence(point);
                statusFence = STATUS_GUI.ADD_POINTS;
                btn_fence.Enabled = true;
            }
            else
            {
                layerRoute.addPosition(point);
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

        //build treeview
        void loadlistPointsTreeView(List<Behavior> list)
        {
            TreeNode root, loc;
            int i = 0;

            root = createRootTreeView(list.Count);

            root.ImageIndex =  (int)IMG_TREEVIEW.FILE;

            foreach (Behavior b in list)
            {
                loc = createLocTreeView(b,root, i++);

                createPositionTreeView(b,loc);

                createEixoTreeView(b.AxisX, "Eixo[X]",loc);
                createEixoTreeView(b.AxisY, "Eixo[Y]",loc);
                createEixoTreeView(b.AxisZ, "Eixo[Z]",loc);
            }
        }

        void showMarkerBehavior(List<Behavior> list)
        {
            foreach(Behavior b in list)
            {
                PointLatLng p = new PointLatLng(b.Position.Latitude, b.Position.Longitude);
                GMarkerGoogleType color = b.OK() ? GMarkerGoogleType.green: GMarkerGoogleType.red;

                StringBuilder sb = new StringBuilder();

                if (!b.OK()) {

                    if (!b.Speed.OK())
                    {
                        sb.Append("Speed:" + b.Speed.ToString());
                    }

                    if (!b.AxisX.OK())
                    {
                        sb.AppendLine();
                        sb.Append("X:"+b.AxisX.ToString());
                    }

                    if (!b.AxisY.OK())
                    {
                        sb.AppendLine();
                        sb.Append("Y:" + b.AxisY.ToString());

                    }

                    if (!b.AxisZ.OK())
                    {
                        sb.AppendLine();
                        sb.Append("Z:" + b.AxisZ.ToString());

                    }
                }

                layerBehavior.addPosition(p,sb.ToString(),color);
            }
        }

        TreeNode createRootTreeView(int num)
        {
            TreeNode root;

            tvBehavior.Nodes.Clear();

            if (tvBehavior.Nodes.Count <= 0)
            {
                root = tvBehavior.Nodes.Add(string.Format("Trip: ({0})",num));
            }
            else
            {

                root = tvBehavior.Nodes[0];
            }

            return root;
        }

        TreeNode createLocTreeView(Behavior behavior, TreeNode root,int i)
        {
            TreeNode loc;

            loc = root.Nodes.Add(string.Format("Registro[{0}] : ({1})", i, behavior.DateTime));

            loc.ImageIndex = (int)(behavior.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);

            return loc;
        }

        void createPositionTreeView(Behavior behavior, TreeNode loc)
        {
            loc.Nodes.Add(string.Format("Localizacao: ({0},{1})",
                                    behavior.Position.Latitude,
                                    behavior.Position.Longitude)).ImageIndex = (int)IMG_TREEVIEW.LOC;

            loc.Nodes.Add(string.Format("Velocidade: {0}", 
                                    behavior.Speed.Val)).ImageIndex = (int)(behavior.Speed.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);

        }

        TreeNode createEixoTreeView(Axis axis, string neixo, TreeNode loc)
        {
            TreeNode eixo;

            eixo = loc.Nodes.Add(string.Format(neixo));

            eixo.ImageIndex = (int)(axis.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);

            eixo.Nodes.Add(string.Format("A: Val:{0} Min:{1} Max:{2}",
                                        axis.Acceleration.Val,
                                        axis.Acceleration.Tol.Min,
                                        axis.Acceleration.Tol.Max)).ImageIndex = (int)IMG_TREEVIEW.SPEC;

            eixo.Nodes.Add(string.Format("R: Val:{0} Min:{1} Max:{2}",
                                        axis.Rotation.Val,
                                        axis.Rotation.Tol.Min,
                                        axis.Rotation.Tol.Max)).ImageIndex = (int)IMG_TREEVIEW.SPEC;
            return eixo;
        }
        //--------------------------------------------------------------------------------------

        List<Behavior> getBehaviorFiltered(int i)
        {
            List<Behavior> ret = null;

            switch (i)
            {
                case 0: ret = trackerController.ListBehavior;   break;
                case 1: ret = trackerController.getItensOK();   break;
                case 2: ret = trackerController.getItensNOK();  break;
            }

            return ret;
        }

        void addFence(Fence fence) {

            string str = string.Format("Fence:{0}", cbListFence.Items.Count);

            layerFence.addFence(str,fence);
            trackerController.addFence(fence);
            cbListFence.Items.Add(str);
        }

        void removeFence(int index)
        {
            layerFence.removeFenceAt(index);
            trackerController.removeFenceAt(index);
            cbListFence.Items.RemoveAt(index);
        }

        private void cbFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<Behavior> list = getBehaviorFiltered(cbFilter.SelectedIndex);

            loadlistPointsTreeView(list);

            showMarkerBehavior(list);
        }

        void removePositionFence(int index)
        {
            fence.removePositionAt(index);
            layerFence.removePositionAt(index);
        }

        void addPositionFence(PointLatLng point)
        {
            fence.insertPositon(point);
            layerFence.addPosition(point);
        }
    }

    class POSITION_CONST
    {
        internal const double LATITUDE = -23.673326;
        internal const double LONGITUDE = -46.775215;
    }

    enum STATUS_GUI {

        INIT,
        INIT_OK,
        NEW_FENCE,
        ADD_POINTS,
        CONFIRM_FENCE
    }

    enum IMG_TREEVIEW
    {
        OK = 0,
        NOK = 1,
        SPEC = 2,
        LOC =3,
        FILE =4
    }
}