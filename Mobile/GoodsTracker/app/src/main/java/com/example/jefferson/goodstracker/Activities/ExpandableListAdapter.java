package com.example.jefferson.goodstracker.Activities; /**
 * Created by Jefferson on 06/07/2017.
 */

import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Typeface;

import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;
import com.example.jefferson.goodstracker.Domain.Tracker;
import com.example.jefferson.goodstracker.R;

public class ExpandableListAdapter extends BaseExpandableListAdapter {

    private Activity                    context;
    private Tracker[]                   trackers;

    public ExpandableListAdapter(Activity context, Tracker[] list) {

        this.context            = context;
        this.trackers            = list;
    }

    public Object getChild(int groupPosition, int childPosition) {
        return groupPosition<trackers.length?trackers[groupPosition]:null;
    }

    public long getChildId(int groupPosition, int childPosition) {
        return childPosition;
    }


    public View getChildView(final int groupPosition, final int childPosition,
                             boolean isLastChild, View convertView, ViewGroup parent) {

        final Tracker tracker   = (Tracker) getGroup(groupPosition);
        DataTelemetria tlm      = tracker.getTlm();

        if (convertView == null) {

            LayoutInflater inflater = context.getLayoutInflater();
            convertView             = inflater.inflate(R.layout.child_status_item, null);
        }

        TextView textLat        = (TextView) convertView.findViewById(R.id.textLat);
        TextView textLng        = (TextView) convertView.findViewById(R.id.textLng);
        TextView valLevel       = (TextView) convertView.findViewById(R.id.valueLevel);
        ProgressBar barLevel    = (ProgressBar)convertView.findViewById(R.id.barLevel);


        textLat.setText(String.format("%f",tlm.getLatitude()));
        textLng.setText(String.format("%f",tlm.getLongitude()));
        valLevel.setText(String.format("%.1f",tlm.getValLevel()));
        barLevel.setMax((int) tlm.getLevel().getTol().getMax());
        barLevel.setProgress((int)tlm.getValLevel());

        return convertView;
    }

    public int getChildrenCount(int groupPosition) {
        return 1;
    }

    public Object getGroup(int groupPosition) {
        return groupPosition<trackers.length?trackers[groupPosition]:null;
    }

    public int getGroupCount() {
        return trackers.length;
    }

    public long getGroupId(int groupPosition) {
        return groupPosition;
    }

    public View getGroupView(int groupPosition, boolean isExpanded,
                             View convertView, ViewGroup parent) {

        Tracker tracker = (Tracker) getGroup(groupPosition);

        if (convertView == null) {

            LayoutInflater infalInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = infalInflater.inflate(R.layout.group_status_item, null);
        }

        TextView item = (TextView) convertView.findViewById(R.id.laptop);
        item.setTypeface(null, Typeface.BOLD);
        item.setText(String.format("Tracker: %05d",tracker.getAddress()));
        return convertView;
    }

    public boolean hasStableIds() {
        return true;
    }

    public boolean isChildSelectable(int groupPosition, int childPosition) {
        return true;
    }
}
