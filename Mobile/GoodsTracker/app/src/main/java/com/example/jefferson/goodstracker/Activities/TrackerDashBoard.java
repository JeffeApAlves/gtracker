package com.example.jefferson.goodstracker.Activities;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.jefferson.goodstracker.Controllers.Tracking;
import com.example.jefferson.goodstracker.Domain.DataTelemetria;
import com.example.jefferson.goodstracker.Domain.Tracker;
import com.example.jefferson.goodstracker.R;

/**
 * Created by Jefferson on 16/07/2017.
 */

public class TrackerDashBoard extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view   = inflater.inflate(R.layout.tracker_dash_board, container, false);

        createAdapter(view);

        return view;
    }

    void createAdapter(View view){

        Tracking    tracking        = Tracking.getInstance();
        ListView    listTrackers    = (ListView)view.findViewById(R.id.listTrackers);

        listTrackers.setAdapter(new ListTrackerAdapter(tracking.getTrackers()));
    }

    class ListTrackerAdapter extends ArrayAdapter<Tracker> {

        public ListTrackerAdapter(Tracker[] trackers) {
            super(getActivity(),R.layout.item_list_tracker,trackers);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent){

            final Tracker tracker   = (Tracker) getItem(position);

            LayoutInflater inflater = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            View    row             = inflater.inflate(R.layout.item_list_tracker, parent,false);

            fillfields(row,tracker);

            return row;
        }

        private void fillfields(View row,Tracker tracker){

            DataTelemetria tlm      = tracker.getTlm();

            TextView textTracker    = (TextView)    row.findViewById(R.id.tracker);
            ImageView stsTracker    = (ImageView)   row.findViewById(R.id.status_tracker);
            TextView textLat        = (TextView)    row.findViewById(R.id.textLat);
            TextView textLng        = (TextView)    row.findViewById(R.id.textLng);
            TextView valLevel       = (TextView)    row.findViewById(R.id.valueLevel);
            ProgressBar barLevel    = (ProgressBar) row.findViewById(R.id.barLevel);

            textTracker.setText(String.format("Tracker: %05d",tracker.getAddress()));
            textLat.setText(String.format("%f",tlm.getLatitude()));
            textLng.setText(String.format("%f",tlm.getLongitude()));
            valLevel.setText(String.format("%.1f",tlm.getValLevel()));
            barLevel.setMax((int) tlm.getLevel().getTol().getMax());
            barLevel.setProgress((int)tlm.getValLevel());
        }
    }


/*
    public static TrackerDashBoard newInstance(Tracking tracking) {

        TrackerDashBoard myFragment = new TrackerDashBoard();

        //Passa o argumento para o Frame
        Bundle args = new Bundle();
        args.putInt("tracking", tracking);
        myFragment.setArguments(args);

        return myFragment;
    }

    recuperando argumento
    getArguments().getInt("tracking", 0);

*/

}
