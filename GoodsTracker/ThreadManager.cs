using System.Threading;


namespace GoodsTracker
{
    public class ThreadManager
    {
        static ThreadCommunication workerCommunication = null;
        static ThreadDomain workerDomain = null;

        static Thread threadCommunication, threadDomain;

        static public void init()
        {
            workerCommunication = new ThreadCommunication();
            threadCommunication = new Thread(workerCommunication.DoWork);

            workerDomain        = new ThreadDomain();
            threadDomain        = new Thread(workerDomain.DoWork);
        }

        static public void deInit()
        {
            stopThreads();
        }

        static public void startThreads()
        {
            threadDomain.Start();
            threadCommunication.Start();
        }

        static public void stopThreads()
        {
            workerCommunication.RequestStop();
            threadCommunication.Join();

            workerDomain.RequestStop();
            threadDomain.Join();
        }
    }
}
