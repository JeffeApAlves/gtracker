package com.example.jefferson.goodstracker.Domain;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import static android.R.attr.value;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class DataTelemetria  extends Object {

    private Map<Integer,Boolean> insideOfFence;

    private double      latitude, longitude;
    private Value       speed;
    private Value       level;
    private Axis        axisX, axisY, axisZ;

    private boolean     statusLock;
    private Date        date;
    private Date        time;

    public DataTelemetria()
    {
        axisX       = new Axis();
        axisY       = new Axis();
        axisZ       = new Axis();
        speed       = new Value(0,100);
        level       = new Value(0,70000);

        insideOfFence = new HashMap<Integer,Boolean>();
    }

    public boolean OK()
    {
        return speed.OK() && axisX.OK() && axisY.OK() && axisZ.OK();
    }

    public void setPosition(double lat,double lng)
    {
        latitude = lat;
        longitude = lng;
    }

    public String getStrNOK()
    {
        StringBuilder sb = new StringBuilder();

        if (!OK())
        {
            if (!speed.OK())
            {
                sb.append("Speed:" + speed.toString());
            }

            if (!axisX.OK())
            {
                sb.append('\n');
                sb.append("X:" + axisX.toString());
            }

            if (!axisY.OK())
            {
                sb.append('\n');
                sb.append("Y:" + axisY.toString());

            }

            if (!axisZ.OK())
            {
                sb.append('\n');
                sb.append("Z:" + axisZ.toString());
            }
        }

        return sb.toString();
    }

    public void setAcceleration(double x, double y, double z)
    {
        axisX.setAcceleration(x);
        axisY.setAcceleration(y);
        axisZ.setAcceleration(z);
    }

    public void setRotation(double x, double y, double z)
    {
        axisX.setRotation(x);
        axisY.setRotation(y);
        axisZ.setRotation(z);
    }

    public void setValues(DataTelemetria values)
    {
        speed   = values.getSpeed();
        level   = values.getLevel();

        axisX   = values.getAxisX();
        axisY   = values.getAxisY();
        axisZ   = values.getAxisZ();
    }

    @Override
    public String toString()
    {
        return String.format("Lat:%f Lng:%f %s %s %s",latitude, longitude,axisX.toString(),axisY.toString(),axisZ.toString());
    }

    public void setSpeed(int v)
    {
        setSpeed(v);
    }

    public void setLevel(int v)
    {
        level.setVal(v);
    }

    public void setInsideOfFence(int index,boolean status)
    {
        insideOfFence.put(index,status);
    }

    public boolean IsInsideOfFence()
    {
        boolean ret = false;

        if (insideOfFence != null) {

            for (Map.Entry<Integer, Boolean> entry : insideOfFence.entrySet()) {

                if (entry.getValue()) {

                    ret = true;
                    break;
                }
            }
        }

        return ret;
    }

    public Map<Integer, Boolean> getInsideOfFence() {
        return insideOfFence;
    }

    public void setInsideOfFence(Map<Integer, Boolean> insideOfFence) {
        this.insideOfFence = insideOfFence;
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }

    public Value getSpeed() {
        return speed;
    }

    public void setSpeed(Value speed) {
        this.speed = speed;
    }

    public Value getLevel() {
        return level;
    }

    public void setLevel(Value level) {
        this.level = level;
    }

    public Axis getAxisX() {
        return axisX;
    }

    public void setAxisX(Axis axisX) {
        this.axisX = axisX;
    }

    public Axis getAxisY() {
        return axisY;
    }

    public void setAxisY(Axis axisY) {
        this.axisY = axisY;
    }

    public Axis getAxisZ() {
        return axisZ;
    }

    public void setAxisZ(Axis axisZ) {
        this.axisZ = axisZ;
    }

    public boolean isStatusLock() {
        return statusLock;
    }

    public void setStatusLock(boolean statusLock) {
        this.statusLock = statusLock;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public void setSpeed(double speed) {
        this.speed.setVal(speed);
    }

    public void setLevel(double level) {
        this.level.setVal(level);
    }

    public double getValSpeed() {

        return speed.getVal();
    }

    public double getValLevel() {

        return level.getVal();
    }
}
