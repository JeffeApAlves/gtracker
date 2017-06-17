using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class TrackerController
    {
        Tracker tracker;

        List<Behavior>  listBehavior;
        List<Fence>     listFence;

        internal List<Behavior> ListBehavior { get => listBehavior; set => listBehavior = value; }
        internal List<Fence> ListFence { get => listFence; set => listFence = value; }

        internal TrackerController()
        {
            tracker         = new Tracker();
            ListFence       = new List<Fence>();
            ListBehavior    = new List<Behavior>();

            testeBehavior();
        }

        internal Fence createFence()
        {
            Fence fence = new Fence();

            return fence;
        }

        internal void addFence(Fence fence)
        {
            listFence.Add(fence);
        }

        internal void removeFenceAt(int index)
        {
            listFence.RemoveAt(index);
        }

        internal List<Behavior> getItensNOK()
        {
            List<Behavior> ret=new List<Behavior>();

            foreach(Behavior b in listBehavior)
            {
                if (!b.OK())
                {
                    ret.Add(b);
                }
            }

            return ret;
        }

        internal List<Behavior> getItensOK()
        {
            List<Behavior> ret = new List<Behavior>();

            foreach (Behavior b in listBehavior)
            {
                if (b.OK())
                {
                    ret.Add(b);
                }
            }

            return ret;
        }

        private void testeBehavior()
        {
            double LATITUDE = -23.673326;
            double LONGITUDE = -46.775215;

            for (int i = 0; i < 100; i++)
            {
                Behavior b = new Behavior();

                b.DateTime = DateTime.Now;

                LATITUDE += -0.001;
                LONGITUDE += 0.001;

                b.setPosition(LATITUDE, LONGITUDE);

                ListBehavior.Add(b);
            }
        }

        internal List<Behavior> getBehaviorFiltered(int i)
        {
            List<Behavior> ret = null;

            switch (i)
            {
                case 0: ret = ListBehavior; break;
                case 1: ret = getItensOK(); break;
                case 2: ret = getItensNOK(); break;
            }

            return ret;
        }
    }
}
