package com.example.jefferson.goodstracker.Activities;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.jefferson.goodstracker.R;

/**
 * Created by Jefferson on 16/07/2017.
 */

public class TrackerHome extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view   = inflater.inflate(R.layout.tracker_home, container, false);

        return view;
    }
}
