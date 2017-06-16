using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using System.Collections.Generic;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        LayerMap layerFence, layerRoute, layerBehavior;

        STATUS_GUI  statusGUI   = STATUS_GUI.INIT;
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
            loadlistPointsTreeView(trackerController.ListBehavior);
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
        //------------------------------------------------------------------------------------

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point   = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            txtLat.Text         = point.Lat.ToString();
            txtLng.Text         = point.Lng.ToString();

            if (statusGUI.Equals(STATUS_GUI.NEW_FENCE) || statusGUI.Equals(STATUS_GUI.ADD_POINTS))
            {
                addPositionFence(point);
                statusGUI = STATUS_GUI.ADD_POINTS;
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
                addFence(fence);

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
            if (statusGUI.Equals(STATUS_GUI.ADD_POINTS))
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

            root = createRootTreeView();

            foreach (Behavior b in list)
            {
                loc = createLocTreeView(root, i++);

                createPositionTreeView(b,loc);

                createEixoTreeView("Eixo[X]", b.AxisX, loc);
                createEixoTreeView("Eixo[Y]", b.AxisY, loc);
                createEixoTreeView("Eixo[Z]", b.AxisZ, loc);
            }
        }

        TreeNode createRootTreeView()
        {
            TreeNode root;

            if (tvBehavior.Nodes.Count <= 0)
            {
                root = tvBehavior.Nodes.Add("Trip");
            }
            else
            {

                root = tvBehavior.Nodes[0];
            }

            return root;
        }

        TreeNode createLocTreeView(TreeNode root,int i)
        {
            TreeNode loc;

            loc = root.Nodes.Add(string.Format("Localizacao[{0}]", i));

            return loc;
        }

        void createPositionTreeView(Behavior behavior, TreeNode loc)
        {
            loc.Nodes.Add(string.Format("Localizacao: ({0},{1})",
                                    behavior.Position.Latitude,
                                    behavior.Position.Longitude));

            loc.Nodes.Add(string.Format("Velocidade: {0}", behavior.Speed.Val));

        }

        TreeNode createEixoTreeView(string neixo,Axis axis, TreeNode loc)
        {
            TreeNode eixo;

            eixo = loc.Nodes.Add(string.Format(neixo));

            eixo.Nodes.Add(string.Format("A: {0} Min:{1} Max:{2}",
                                        axis.Acceleration.Val,
                                        axis.Acceleration.Tol.Min,
                                        axis.Acceleration.Tol.Max));

            eixo.Nodes.Add(string.Format("R: {0} Min:{1} Max:{2}",
                                        axis.Rotation.Val,
                                        axis.Rotation.Tol.Min,
                                        axis.Rotation.Tol.Max));
            return eixo;
        }
        //--------------------------------------------------------------------------------------

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