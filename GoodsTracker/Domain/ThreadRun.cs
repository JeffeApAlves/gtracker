﻿using System;
using System.Threading;


namespace GoodsTracker
{
    public delegate void onUpdate();

    abstract class ThreadRun
    {
        protected Thread thread;

        private int time_ms = 1000;

        private volatile bool _shouldStop;

        public void DoWork()
        {
            while (!_shouldStop)
            {
                run();

                Thread.Sleep(time_ms);
            }
        }

        public ThreadRun()
        {
            thread = new Thread(DoWork);

            ThreadManager.add(this);
        }

        abstract public void run();

        public void RequestStop()
        {
            _shouldStop = true;
        }

        public void start()
        {
            thread.Start();
        }

        public void setTime(int time)
        {
            time_ms = time;
        }

        public void join()
        {
            thread.Join();
        }
    }
}
