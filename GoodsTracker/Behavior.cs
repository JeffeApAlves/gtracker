using System;
using System.Text;

namespace GoodsTracker
{
    internal class Scale {

        double min;
        double max;

        internal double Min { get => min; set => min = value; }
        internal double Max { get => max; set => max = value; }

        internal Scale(double _min,double _max)
        {
            min = _min;
            max = _max;
        }
    }

    internal class Value
    {
        double  val;
        Scale   tol;

        internal double   Val { get => val; set => val = value; }
        internal Scale    Tol { get => tol; set => tol = value; }

        internal Value(double min,double max)
        {
            tol = new Scale(min,max);
        }
        
        internal Value()
        {
            tol = new Scale(0,0);
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

        internal Axis()
        {
            Acceleration    = new Value(0,90);
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

        internal string getStrNOK()
        {
            StringBuilder sb = new StringBuilder();

            if (!OK())
            {
                if (!speed.OK())
                {
                    sb.Append("Speed:" + speed.ToString());
                }

                if (!axisX.OK())
                {
                    sb.AppendLine();
                    sb.Append("X:" + axisX.ToString());
                }

                if (!axisY.OK())
                {
                    sb.AppendLine();
                    sb.Append("Y:" + axisY.ToString());

                }

                if (!axisZ.OK())
                {
                    sb.AppendLine();
                    sb.Append("Z:" + axisZ.ToString());
                }
            }

            return sb.ToString();
        }

        internal void setAcceleration(double x, double y, double z)
        {
            AxisX.Acceleration.Val = x;
            AxisY.Acceleration.Val = y;
            AxisZ.Acceleration.Val = z;
        }
    }
}