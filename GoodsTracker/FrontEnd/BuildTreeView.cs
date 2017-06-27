using System;
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
            LEVEL = 11,
            LOCK = 10,
        }

        bool forceChange    = false;
        bool forceClear     = false;

        TreeView treeView;
        TelemetriaData[] behaviors;

        int filter = 0;

        public TreeView TreeView { get => treeView; set => treeView = value; }

        public TelemetriaData[] Behaviors
        {
            get
            {
                return behaviors;
            }

            set
            {
                if (behaviors == null & value != null)
                {
                    forceChange = true;
                    forceClear  = true;

                } else if(behaviors == null & value == null)
                {
                    forceChange = false;
                    forceClear  = false;
                }
                else if (behaviors.Length <= 0)
                {
                    forceChange = true;
                    forceClear  = true;
                }
                else
                {
                    if (behaviors.All(value.Contains))
                    {
                        forceChange = true;
                    }

                    if(!ContainsEquivalentSequence(behaviors,value))
                    {
                        forceClear = true;
                    }
                }

                behaviors = value;
            }
        }

        public bool ContainsEquivalentSequence<T>(T[] array1, T[] array2)
        {
            bool a1IsNullOrEmpty = ReferenceEquals(array1, null) || array1.Length == 0;
            bool a2IsNullOrEmpty = ReferenceEquals(array2, null) || array2.Length == 0;

            if (a1IsNullOrEmpty)
            {
                return a2IsNullOrEmpty;
            }

            if (a2IsNullOrEmpty)
            {
                return false;
            }

            for (int i = 0; i < array1.Length; i++)
                if (!Equals(array1[i], array2[i]))
                    return false;

            return true;
        }

        public int Filter
        {
            get
            {
                return filter;
            }

            set
            {
                if (filter != value)
                {
                    filter = value;
                    forceClear = true;
                }
            }
        }

        internal BuildTreeView(TreeView tv)
        {
            treeView    = tv;
            behaviors   = null;
            filter      = 0;

            update();
        }

        internal void update()
        {
            if (forceClear)
            {
                loadlistPointsTreeView();
            }
            else if (forceChange)
            {
                addPointsTreeView();
            }
        }

        /**
         * Adicona apenas novos pontos
         * 
         */
        internal void addPointsTreeView()
        {
            forceChange = false;

            if (behaviors != null)
            {
                TreeNode root = createRootTreeView();

                // Adiciona a partir do ultimo i =getCount();
                for (int i = getCount(); i < behaviors.Length; i++)
                {
                    addTelemetria(behaviors[i],root);
                }
            }
        }

        /*
         * Constroe arvore do historico da viagem
         *  
         */
        internal void loadlistPointsTreeView()
        {
            forceChange = false;

            if (behaviors != null)
            {
                clear();

                TreeNode root = createRootTreeView();

                foreach (TelemetriaData b in behaviors)
                {
                    addTelemetria(b,root);
                }
            }
        }

        /*
         * 
         * 
         */
        internal int getCount()
        {
            return TreeView.Nodes.Count <= 0 ? 0 : TreeView.Nodes[0].Nodes.Count;
        }

        internal void addTelemetria(TelemetriaData b,TreeNode    root)
        {
            if (b != null)
            {
                TreeNode loc;

                loc = createLocTreeView(b, root);

                createPositionTreeView(b, loc);
                createLockTreeView(b, loc);
                createLevelTreeView(b, loc);
                createSpeedTreeView(b, loc);
                createEixoTreeView(b.AxisX, "Eixo[X]", loc);
                createEixoTreeView(b.AxisY, "Eixo[Y]", loc);
                createEixoTreeView(b.AxisZ, "Eixo[Z]", loc);
            }
        }

        internal void clear()
        {
            forceClear = false;

            if (treeView.Nodes.Count > 0 && treeView.Nodes[0] != null)
            {
                treeView.Nodes[0].Nodes.Clear();
            }
        }

        internal TreeNode createRootTreeView()
        {
            TreeNode root;

            /*
             *   |
             * Trip
             * 
             *   
             */
            if (treeView.Nodes.Count <= 0)
            {
                root = treeView.Nodes.Add(string.Format("Trip"));
            }
            else
            {
                root = treeView.Nodes[0];
            }

            root.ImageIndex = (int)IMG_TREEVIEW.FILE;
            root.SelectedImageIndex = (int)IMG_TREEVIEW.FILE;

            return root;
        }

        internal TreeNode createLocTreeView(TelemetriaData b, TreeNode root)
        {
            TreeNode loc;

            /*
             *   |
             * Registro[XXXXX]: 00/00/00 00:00
             *   |__
             *
             */
            loc = root.Nodes.Add(string.Format("Registro[{0}]: {1}", root.Nodes.Count.ToString("D5"), b.DateTime));
                loc.ImageIndex = (int)(b.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
                loc.SelectedImageIndex = loc.ImageIndex;

            return loc;
        }

        internal void createPositionTreeView(TelemetriaData b, TreeNode reg)
        {
            TreeNode info, loc;

            /*
             *   |
             * Localizacao
             *   |__Lat
             *   |__Lng
             */
            loc = reg.Nodes.Add("Localizacao");
            loc.ImageIndex = (int)IMG_TREEVIEW.LOC;
            loc.SelectedImageIndex = loc.ImageIndex;

                info = loc.Nodes.Add(string.Format("Lat: {0}", b.Latitude));
                info.ImageIndex = (int)IMG_TREEVIEW.PUSHPIN;
                info.SelectedImageIndex = info.ImageIndex;

                info = loc.Nodes.Add(string.Format("Lng: {0}", b.Longitude));
                info.ImageIndex = (int)IMG_TREEVIEW.PUSHPIN;
                info.SelectedImageIndex = info.ImageIndex;
        }

        internal void createSpeedTreeView(TelemetriaData b, TreeNode reg)
        {
            TreeNode info, vel;

            /*
             *   |
             * Velocidade
             *   |__Valor
             */
            vel = reg.Nodes.Add("Velocidade");
            vel.ImageIndex = (int)IMG_TREEVIEW.GUAUGE;
            vel.SelectedImageIndex = vel.ImageIndex;

                info = vel.Nodes.Add(b.Speed.ToString());
                info.ImageIndex = (int)(b.Speed.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
                info.SelectedImageIndex = info.ImageIndex;
        }

        internal void createLevelTreeView(TelemetriaData b, TreeNode reg)
        {
            TreeNode info, level;

            /*
             *   |
             * Nivel
             *   |__Valor
             */
            level = reg.Nodes.Add("Nivel");
            level.ImageIndex = (int)IMG_TREEVIEW.LEVEL;
            level.SelectedImageIndex = level.ImageIndex;

                info = level.Nodes.Add(b.Level.ToString());
                info.ImageIndex = (int)(b.Level.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
                info.SelectedImageIndex = info.ImageIndex;
        }

        internal void createLockTreeView(TelemetriaData b, TreeNode reg)
        {
            TreeNode info, _lock;

            /*
             *   |
             * Trava
             *   |__Valor
             */
            _lock = reg.Nodes.Add("Trava");
            _lock.ImageIndex = (int)IMG_TREEVIEW.LOCK;
            _lock.SelectedImageIndex = _lock.ImageIndex;

                info = _lock.Nodes.Add(b.StatusLock.ToString());
                info.ImageIndex = (int)(b.StatusLock? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
                info.SelectedImageIndex = info.ImageIndex;
        }

        internal TreeNode createEixoTreeView(Axis axis, string neixo, TreeNode loc)
        {
            TreeNode eixo, spec;

            /*
             *   |
             * Eixo[x]
             *   |__ A: Val:0 Min:0 Max:0
             *   |__ N: Val:0 Min:0 Max:0
             */
            eixo = loc.Nodes.Add(string.Format(neixo));
            eixo.ImageIndex = (int)IMG_TREEVIEW.AXIS;
            eixo.SelectedImageIndex = (int)IMG_TREEVIEW.AXIS;


            spec = eixo.Nodes.Add(string.Format("A: Val:{0} Min:{1} Max:{2}",
                                        axis.Acceleration.Val,
                                        axis.Acceleration.Tol.Min,
                                        axis.Acceleration.Tol.Max));
            spec.ImageIndex = (int)(axis.Acceleration.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
            spec.SelectedImageIndex = spec.ImageIndex;

            spec = eixo.Nodes.Add(string.Format("N: Val:{0} Min:{1} Max:{2}",
                                        axis.Rotation.Val,
                                        axis.Rotation.Tol.Min,
                                        axis.Rotation.Tol.Max));
            spec.ImageIndex = (int)(axis.Rotation.OK() ? IMG_TREEVIEW.OK : IMG_TREEVIEW.NOK);
            spec.SelectedImageIndex = spec.ImageIndex;

            return eixo;
        }

        private void test()
        {
            List<TelemetriaData> be = new List<TelemetriaData>();
            List<TelemetriaData> v = new List<TelemetriaData>();

            TelemetriaData e1 = new TelemetriaData();
            TelemetriaData e2 = new TelemetriaData();
            TelemetriaData e3 = new TelemetriaData();
            TelemetriaData e4 = new TelemetriaData();

            be.Add(e1);
            be.Add(e2);
            be.Add(e3);
            //            be.Add(e4);

            v.Add(e1);
            v.Add(e2);
            v.Add(e3);
            v.Add(e4);

            Console.WriteLine("{0}", be.ToArray().All(v.ToArray().Contains));
            Console.WriteLine("{0}", !be.ToArray().SequenceEqual(v.ToArray()));
        }
    }
}

