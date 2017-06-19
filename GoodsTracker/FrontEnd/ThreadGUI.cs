using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class ThreadGUI : ThreadRun
    {
        update update;

        public override void run()
        {
            if (update != null)
            {
                update();
            }
        }

        public void setUpdate(update func)
        {
            update = func;
        }
    }
}
