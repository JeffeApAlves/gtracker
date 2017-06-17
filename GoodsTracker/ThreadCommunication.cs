using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class ThreadCommunication
    {
        private volatile bool _shouldStop;

        public void DoWork()
        {
            while (!_shouldStop)
            {
                Thread.Sleep(100);
            }
        }

        public void RequestStop()
        {
            _shouldStop = true;
        }
    }
}
