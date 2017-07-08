package com.example.jefferson.goodstracker.Domain;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Axis extends Object{

    Value acceleration, rotation;

    public Axis()
    {
        acceleration    = new Value(0,15);
        rotation        = new Value(15,30);
    }

    public boolean OK()
    {
        return acceleration.OK() && rotation.OK();
    }

    @Override
    public String toString()
    {
        return "A: "+acceleration.toString() + " R: " + rotation.toString();
    }

    public Value getAcceleration() {
        return acceleration;
    }

    public void setAcceleration(Value acceleration) {
        this.acceleration = acceleration;
    }

    public Value getRotation() {
        return rotation;
    }

    public void setRotation(Value rotation) {
        this.rotation = rotation;
    }

    public void setRotation(double v) {

        rotation.setVal(v);
    }

    public void setAcceleration(double v) {

        acceleration.setVal(v);
    }
}
