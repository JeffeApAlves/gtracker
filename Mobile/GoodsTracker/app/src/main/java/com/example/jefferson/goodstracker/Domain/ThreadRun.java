package com.example.jefferson.goodstracker.Domain;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Jefferson on 08/07/2017.
 */

public abstract class ThreadRun extends Object {

    private static List<ThreadRun> threads = new ArrayList<ThreadRun>();
    private Thread thread;
    private int time_ms = 10000;
    private volatile boolean _shouldStop;

    public void DoWork() {

        while (!_shouldStop) {

            try {

                run();

                Thread.sleep(time_ms);
            }
            catch (Exception e)
            {
                System.out.println("Erro na execucao de alguma Thread");
                System.out.println(e.toString());
                Log.d("",e.toString());

                _shouldStop = true;
                break;
            }
        }
    }

    public ThreadRun() {

        thread = new Thread(new Runnable() {

            @Override
            public void run() {

                DoWork();
            }

        });

        add(this);
    }

    abstract public void run();

    public void RequestStop()
    {
        _shouldStop = true;
    }

    public void start()
    {
        thread.start();
    }

    public void setTime(int time)
    {
        time_ms = time;
    }

    public void join() throws InterruptedException {

        thread.join();
    }

    static public void add(ThreadRun t) {

        if (t != null) {

            threads.add(t);
        }
    }

    static public void startAll() {

        for (ThreadRun t : threads) {

            t.start();
        }
    }

    static public void stopAll() throws InterruptedException {

        for (ThreadRun t : threads) {

            t.RequestStop();
            t.join();
        }
    }
}
