package com.example.jefferson.goodstracker.Domain;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Value extends Object {

    double  val;
    Scale   tol;

    public Value(double min,double max) {

        tol = new Scale(min,max);
    }

    public Value() {

        tol = new Scale(0,0);
    }

    public boolean OK() {

        return val>=tol.getMin() && val<=tol.getMax();
    }

    @Override
    public String toString() {

        return Double.toString(val);
    }

    public double getVal() {
        return val;
    }

    public void setVal(double val) {
        this.val = val;
    }

    public Scale getTol() {
        return tol;
    }

    public void setTol(Scale tol) {
        this.tol = tol;
    }
}
