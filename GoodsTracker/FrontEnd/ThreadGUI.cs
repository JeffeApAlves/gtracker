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

        public onUpdate OnUpdate { get => onUpdate; set => onUpdate = value; }

        public override void run()
        {
            onUpdate?.Invoke();
        }
    }
}
