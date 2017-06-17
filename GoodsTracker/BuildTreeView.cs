using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace GoodsTracker
{
    internal class BuildTreeView
    {
        enum IMG_TREEVIEW
        {
            OK = 0,
            NOK = 1,
            SPEC = 2,
            LOC = 3,
            FILE = 4,
            INFO = 5,
            AXIS = 6,
            GUAUGE = 7,
            PUSHPIN = 8,
            XYZ = 9,
        }

        TreeView treeView;

        internal BuildTreeView(TreeView tv)
        {
            treeView = tv;
        }

        /*
         * Constroe arvore do historico da viagem
         *  
         */
        internal void loadlistPointsTreeView(List<Behavior> listBehavior)
        {
            TreeNode root, loc;

            int i = 0;

            root = createRootTreeView(listBehavior.Count);

            foreach (Behavior b in listBehavior)
            {
                loc = createLocTreeView(b, root, i++);

                createPositionTreeView(b, loc);

                createEixoTreeView(b.AxisX, "Eixo[X]", loc);
                createEixoTreeView(b.AxisY, "Eixo[Y]", loc);
                createEixoTreeView(b.AxisZ, "Eixo[Z]", loc);
            }
        }

        internal TreeNode createRootTreeView(int num)
        {
            TreeNode root;

            treeView.Nodes.Clear();

            if (treeView.Nodes.Count <= 0)
            {
                root = treeView.Nodes.Add(string.Format("Trip: ({0})", num));
            }
            else
            {

                root = treeView.Nodes[0];
            }

            root.ImageIndex = (int)IMG_TREEVIEW.FILE;
            root.SelectedImageIndex = (int)IMG_TREEVIEW.FILE;

            return root;
        }

        internal TreeNode createLocTreeView(Behavior behavior, TreeNode root, int i)
        {
            TreeNode loc;

            loc = root.Nodes.Add(string.Format("Registro[{0}]: {1}", i.ToString("D5"), behavior.DateTime));

            loc.ImageIndex = (int)(behavior.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
            loc.SelectedImageIndex = loc.ImageIndex;

            return loc;
        }

        internal void createPositionTreeView(Behavior behavior, TreeNode reg)
        {
            TreeNode info, loc, vel;

            loc = reg.Nodes.Add("Localizacao");
            loc.ImageIndex = (int)IMG_TREEVIEW.LOC;
            loc.SelectedImageIndex = loc.ImageIndex;

            info = loc.Nodes.Add(string.Format("Lat: {0}", behavior.Position.Latitude));
            info.ImageIndex = (int)IMG_TREEVIEW.PUSHPIN;
            info.SelectedImageIndex = info.ImageIndex;

            info = loc.Nodes.Add(string.Format("Lng: {0}", behavior.Position.Longitude));
            info.ImageIndex = (int)IMG_TREEVIEW.PUSHPIN;
            info.SelectedImageIndex = info.ImageIndex;

            vel = reg.Nodes.Add("Velocidade");
            vel.ImageIndex = (int)IMG_TREEVIEW.GUAUGE;
            vel.SelectedImageIndex = vel.ImageIndex;

            info = vel.Nodes.Add(behavior.Speed.ToString());
            info.ImageIndex = (int)(behavior.Speed.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
            info.SelectedImageIndex = info.ImageIndex;
        }

        internal TreeNode createEixoTreeView(Axis axis, string neixo, TreeNode loc)
        {
            TreeNode eixo, spec;

            eixo = loc.Nodes.Add(string.Format(neixo));

            eixo.ImageIndex = (int)IMG_TREEVIEW.AXIS;
            eixo.SelectedImageIndex = (int)IMG_TREEVIEW.AXIS;


            spec = eixo.Nodes.Add(string.Format("A: Val:{0} Min:{1} Max:{2}",
                                        axis.Acceleration.Val,
                                        axis.Acceleration.Tol.Min,
                                        axis.Acceleration.Tol.Max));

            spec.ImageIndex = (int)(axis.Acceleration.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
            spec.SelectedImageIndex = spec.ImageIndex;

            spec = eixo.Nodes.Add(string.Format("R: Val:{0} Min:{1} Max:{2}",
                                        axis.Rotation.Val,
                                        axis.Rotation.Tol.Min,
                                        axis.Rotation.Tol.Max));

            spec.ImageIndex = (int)(axis.Rotation.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
            spec.SelectedImageIndex = spec.ImageIndex;

            return eixo;
        }
    }
}
