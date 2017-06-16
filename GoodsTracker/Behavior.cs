using System;

namespace GoodsTracker
{
    internal class Scale {

        double min;
        double max;

        public double Min { get => min; set => min = value; }
        public double Max { get => max; set => max = value; }
    }

    internal class Value
    {

        double val;
        Scale tol;

        public double Val { get => val; set => val = value; }
        internal Scale Tol { get => tol; set => tol = value; }

        public Value()
        {
            tol = new Scale();
        }
    }

    internal class Axis
    {
        Value acceleration, rotation;

        public Axis()
        {
            Acceleration = new Value();

            Rotation = new Value();
        }

        internal Value Acceleration { get => acceleration; set => acceleration = value; }
        internal Value Rotation { get => rotation; set => rotation = value; }
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

        public Behavior()
        {
            position        = new GPSPosition();
            speed           = new Value();
            AxisX = new Axis();
            AxisY = new Axis();
            AxisZ = new Axis();
        }
    }
}