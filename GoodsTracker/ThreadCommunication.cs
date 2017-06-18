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

        private Protocol communication = Protocol.Communication;

        public void DoWork()
        {
            while (!_shouldStop)
            {
                communication.process();

                Thread.Sleep(2000);
            }
        }

        public void RequestStop()
        {
            _shouldStop = true;
        }
    }
}
