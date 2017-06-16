using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class Tracker
    {
        List<Behavior> listBehavior;

        public Tracker()
        {
            ListBehavior = new List<Behavior>();

            for(int i = 0; i < 100; i++) {

                listBehavior.Add(new Behavior());
            }
        }

        internal List<Behavior> ListBehavior { get => listBehavior; set => listBehavior = value; }
    }
}
