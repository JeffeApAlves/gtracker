using System;

namespace GoodsTracker
{
    internal class Scale {

        double min;
        double max;

        internal double Min { get => min; set => min = value; }
        internal double Max { get => max; set => max = value; }
    }

    internal class Value
    {
        double  val;
        Scale   tol;

        internal double   Val { get => val; set => val = value; }
        internal Scale    Tol { get => tol; set => tol = value; }

        public Value()
        {
            tol = new Scale();
        }

        internal bool OK()
        {
            return Val>=tol.Min && Val<=tol.Max;
        }

        public override string ToString()
        {
            return val.ToString();
        }
    }

    internal class Axis
    {
        Value acceleration, rotation;

        public Axis()
        {
            Acceleration    = new Value();
            Rotation        = new Value();
        }

        internal Value Acceleration { get => acceleration; set => acceleration = value; }
        internal Value Rotation { get => rotation; set => rotation = value; }

        internal bool OK()
        {
            return acceleration.OK() && rotation.OK();
        }

        public override string ToString()
        {
            return "A: "+acceleration.ToString() + " R: " + rotation.ToString();
        }
    }

    internal class Behavior
    {
        GPSPosition position;
        Value       speed;
        Axis        axisX, axisY, axisZ;
        DateTime    dateTime;

        internal Value Speed { get => speed; set => speed = value; }
        internal GPSPosition Position { get => position; set => position = value; }
        internal Axis AxisX { get => axisX; set => axisX = value; }
        internal Axis AxisY { get => axisY; set => axisY = value; }
        internal Axis AxisZ { get => axisZ; set => axisZ = value; }
        public DateTime DateTime { get => dateTime; set => dateTime = value; }

        internal Behavior()
        {
            position    = new GPSPosition();
            speed       = new Value();
            AxisX       = new Axis();
            AxisY       = new Axis();
            AxisZ       = new Axis();
        }

        internal bool OK()
        {
            return speed.OK() && axisX.OK() && axisY.OK() && axisZ.OK();
        }

        internal void setPosition(double lat,double lng)
        {
            position.Latitude = lat;
            position.Longitude = lng;
        }

        public override string ToString()
        {
            return Position.ToString() + " " + axisX.ToString() + " " + axisY.ToString() + " " + axisZ.ToString();
        }
    }
}