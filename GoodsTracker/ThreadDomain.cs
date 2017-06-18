using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class ThreadDomain
    {
        private TrackerController trackerController = TrackerController.TrackerCtrl;
        private volatile bool _shouldStop;

        public void DoWork()
        {
            while (!_shouldStop)
            {
                trackerController.process();

                Thread.Sleep(100);
            }
        }

        public void RequestStop()
        {
            _shouldStop = true;
        }
    }
}
