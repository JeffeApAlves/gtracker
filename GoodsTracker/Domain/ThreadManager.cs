using System.Collections.Generic;

namespace GoodsTracker
{
    public class ThreadManager
    {
        static List<ThreadRun> threads = new List<ThreadRun>();

        static internal void add(ThreadRun t)
        {
            if (t != null)
            {
                threads.Add(t);
            }
        }

        static public void deInit()
        {
            stop();
        }

        static public void start()
        {
            foreach(ThreadRun t in threads)
            {
                t.start();
            }
        }

        static public void stop()
        {
            foreach (ThreadRun t in threads)
            {
                t.RequestStop();
                t.join();
            }
        }
    }
}
