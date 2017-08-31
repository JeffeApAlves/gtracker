using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace GoodsTracker
{
    abstract class ThreadRun
    {
        static List<ThreadRun> threads = new List<ThreadRun>();

        protected Thread thread;
        private int time_ms = 10000;
        private volatile bool _shouldStop;

        public void DoWork()
        {
            while (!_shouldStop)
            {
                try
                {
                    run();

                    Thread.Sleep(time_ms);
                }
                catch (Exception e)
                {
                    Debug.WriteLine("Erro na execucao de alguma Thread");
                    Debug.WriteLine(e.ToString());

                    _shouldStop = true;
                    break;
                }
            }
        }

        public ThreadRun()
        {
            thread = new Thread(DoWork);

            add(this);
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
            if (thread.IsAlive)
            {
                thread.Join();
            }
        }

        static internal void add(ThreadRun t)
        {
            if (t != null)
            {
                threads.Add(t);
            }
        }

        static public void startAll()
        {
            foreach (ThreadRun t in threads)
            {
                t.start();
            }
        }

        static public void stopAll()
        {
            foreach (ThreadRun t in threads)
            {
                t.RequestStop();
                t.join();
            }
        }
    }
}
