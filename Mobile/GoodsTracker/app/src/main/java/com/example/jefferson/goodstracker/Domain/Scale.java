package com.example.jefferson.goodstracker.Domain;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Scale  extends Object {

    double min;
    double max;

    public Scale(double _min,double _max)
    {
        min = _min;
        max = _max;
    }

    public double getMin() {
        return min;
    }

    public void setMin(double min) {
        this.min = min;
    }

    public double getMax() {
        return max;
    }

    public void setMax(double max) {
        this.max = max;
    }
}
