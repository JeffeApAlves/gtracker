using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class ThreadGUI : ThreadRun
    {
        onUpdate onUpdate;

        public override void run()
        {
            onUpdate?.Invoke();
        }

        public void setUpdate(onUpdate func)
        {
            onUpdate = func;
        }
    }
}
