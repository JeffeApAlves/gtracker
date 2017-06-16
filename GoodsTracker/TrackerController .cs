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
        List<Fence> containerFence;
        private List<Fence> listFence;

        internal List<Behavior> ListBehavior { get => listBehavior; set => listBehavior = value; }
        internal List<Fence> ContainerFence { get => containerFence; set => containerFence = value; }
        internal List<Fence> ListFence { get => listFence; set => listFence = value; }

        public TrackerController()
        {
            tracker         = new Tracker();
            ListFence       = new List<Fence>();
            ListBehavior    = new List<Behavior>();

            for (int i = 0; i < 100; i++)
            {
                Behavior b = new Behavior();

                b.DateTime = DateTime.Now;

                ListBehavior.Add(b);
            }
        }

        public Fence createFence()
        {
            Fence fence = new Fence();

            return fence;
        }

        public void addFence(Fence fence)
        {
            listFence.Add(fence);
        }

        public void removeFenceAt(int index)
        {
            listFence.RemoveAt(index);
        }
    }
}
