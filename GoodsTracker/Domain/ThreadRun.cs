using System;
using System.Diagnostics;
using System.Threading;


namespace GoodsTracker
{
    abstract class ThreadRun
    {
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
                    Console.WriteLine("Erro na execucao de alguma Thread");
                    Console.WriteLine(e.ToString());
                    Debug.WriteLine(e.ToString());
                }
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
