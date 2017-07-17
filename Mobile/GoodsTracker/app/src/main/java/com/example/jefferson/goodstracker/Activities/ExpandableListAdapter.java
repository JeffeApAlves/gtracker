package com.example.jefferson.goodstracker.Activities; /**
 * Created by Jefferson on 06/07/2017.
 */

import android.app.Activity;
import android.content.Context;
import android.graphics.Typeface;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;
import com.example.jefferson.goodstracker.Domain.Notification;
import com.example.jefferson.goodstracker.Domain.Tracker;
import com.example.jefferson.goodstracker.R;

import java.util.Map;

//http://theopentutorials.com/tutorials/android/listview/android-expandable-list-view-example/

public class ExpandableListAdapter extends BaseExpandableListAdapter {

    private Activity    context;
    private Tracker[]   trackers;

    public ExpandableListAdapter(Activity context, Tracker[] list) {

        this.context    = context;
        this.trackers   = list;
    }

    public Object getChild(int groupPosition, int childPosition) {

        return trackers[groupPosition].getNotifications()[childPosition];
    }

    public long getChildId(int groupPosition, int childPosition) {
        return childPosition;
    }


    public View getChildView(final int groupPosition, final int childPosition,
                             boolean isLastChild, View convertView, ViewGroup parent) {

        if (convertView == null) {

            LayoutInflater inflater = context.getLayoutInflater();
            convertView             = inflater.inflate(R.layout.child_status_item, null);
        }

        Notification notification   = (Notification)getChild(groupPosition,childPosition);

        TextView textMsg        = (TextView) convertView.findViewById(R.id.textMsg);

        textMsg.setText(notification.getMessage());

        return convertView;
    }

    public int getChildrenCount(int groupPosition) {
        return trackers[groupPosition].getNotifications().length;
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

    public View getGroupView(int groupPosition, boolean isExpanded, View convertView, ViewGroup parent) {

        if (convertView == null) {

            LayoutInflater infalInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = infalInflater.inflate(R.layout.group_status_item, null);
        }

        Tracker tracker = (Tracker) getGroup(groupPosition);

        TextView item = (TextView) convertView.findViewById(R.id.name_tracker);
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
