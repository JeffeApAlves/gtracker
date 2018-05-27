package com.example.jefferson.goodstracker.Activities;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListView;

import com.example.jefferson.goodstracker.Controllers.Tracking;
import com.example.jefferson.goodstracker.R;

/**
 * Created by Jefferson on 16/07/2017.
 */

public class TrackerNotification extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View view   = inflater.inflate(R.layout.tracker_notification, container, false);

        createList(view);

        return view;
    }

    private void createList(View view) {

        Tracking tracking   = Tracking.getInstance();

        ExpandableListView  expListView;

        expListView  = (ExpandableListView) view.findViewById(R.id.laptop_list);

        ExpandableListAdapter expListAdapter = new ExpandableListAdapter(getActivity(), tracking.getTrackers());

        expListView.setAdapter(expListAdapter);

/*
        expListView.setOnChildClickListener(new OnChildClickListener() {

            public boolean onChildClick(ExpandableListView parent, View v,
                                        int groupPosition, int childPosition, long id) {
                final String selected = (String) expListAdapter.getChild(
                        groupPosition, childPosition);

                Toast.makeText(getActivity(), selected, Toast.LENGTH_LONG).show();

                return true;
            }
        });*/
    }
}
