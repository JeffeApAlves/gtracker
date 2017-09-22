package com.example.jefferson.goodstracker.Domain;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Value extends Object {

    private double  val;
    private Tol     tol;

    public Value(double min,double max) {

        tol = new Tol(min,max);
    }

    public Value() {

        tol = new Tol(0,0);
    }

    public boolean OK() {

        return tol.OK(val);
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

    public Tol getTol() {
        return tol;
    }

    public void setTol(Tol tol) {
        this.tol = tol;
    }
}
