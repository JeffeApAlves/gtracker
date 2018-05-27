package com.example.jefferson.goodstracker.Controllers;

import com.example.jefferson.goodstracker.Domain.Tracker;

import java.io.IOException;

/**
 * Created by Jefferson on 16/07/2017.
 */

public class Tracking {

    static Tracking    tracking = null;

    Tracker[]   trackers;

    private Tracking(){

        createTrackers();
    }

    static public Tracking getInstance(){

        if(tracking==null){

            tracking = new Tracking();
        }

        return tracking;
    }

    public Tracker[] getTrackers() {
        return trackers;
    }

    public void createTrackers(){

        try {

            trackers = new Tracker[5];

            for(int i=0;i<trackers.length;i++){

                trackers[i] = new Tracker(i+1);
                trackers[i].requestBehavior();
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
