using System;
using System.Collections.Generic;
using System.Text;

namespace GoodsTracker
{
    class Scale {

        double min;
        double max;

        public double Min { get => min; set => min = value; }
        public double Max { get => max; set => max = value; }

        public Scale(double _min,double _max)
        {
            min = _min;
            max = _max;
        }
    }

    class Value
    {
        double val;
        Scale tol;

        public double Val { get => val; set => val = value; }
        public Scale Tol { get => tol; set => tol = value; }

        public Value(double min,double max)
        {
            tol = new Scale(min,max);
        }
        
        public Value()
        {
            tol = new Scale(0,0);
        }

        public bool OK()
        {
            return Val>=tol.Min && Val<=tol.Max;
        }

        public override string ToString()
        {
            return val.ToString();
        }
    }

    class Axis
    {
        Value acceleration, rotation;

        public Axis()
        {
            Acceleration    = new Value(0,15);
            Rotation        = new Value(15,30);
        }

        public Value Acceleration { get => acceleration; set => acceleration = value; }
        public Value Rotation { get => rotation; set => rotation = value; }

        public bool OK()
        {
            return acceleration.OK() && rotation.OK();
        }

        public override string ToString()
        {
            return /*"A: "+acceleration.ToString() + */ " G: " + rotation.ToString();
        }
    }

    class Telemetria
    {
        Dictionary<int,bool>    insideOfFence;

        double      latitude, longitude;
        Value       speed;
        Value       level;
        Axis        axisX, axisY, axisZ;

        bool        statusLock;
        DateTime    date;
        DateTime    time;

        public Value Speed { get => speed; set => speed = value; }
        public Axis AxisX { get => axisX; set => axisX = value; }
        public Axis AxisY { get => axisY; set => axisY = value; }
        public Axis AxisZ { get => axisZ; set => axisZ = value; }
        public double Latitude { get => latitude; set => latitude = value; }
        public double Longitude { get => longitude; set => longitude = value; }
        public Value Level { get => level; set => level = value; }
        public Dictionary<int, bool> InsideOfFence { get => insideOfFence; set => insideOfFence = value; }
        public bool StatusLock { get => statusLock; set => statusLock = value; }
        public DateTime Date { get => date; set => date = value; }
        public DateTime Time { get => time; set => time = value; }

        public Telemetria()
        {
            axisX       = new Axis();
            axisY       = new Axis();
            axisZ       = new Axis();
            speed       = new Value(0,100);
            level       = new Value(0,70000);

            insideOfFence = new Dictionary<int, bool>();
        }

        public bool OK()
        {
            return speed.OK() && axisX.OK() && axisY.OK() && axisZ.OK();
        }

        public void setPosition(double lat,double lng)
        {
            latitude = lat;
            longitude = lng;
        }

        public string getStrNOK()
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

        public void setAcceleration(double x, double y, double z)
        {
            AxisX.Acceleration.Val = x;
            AxisY.Acceleration.Val = y;
            AxisZ.Acceleration.Val = z;
        }

        public void setRotation(double x, double y, double z)
        {
            axisX.Rotation.Val = x;
            axisY.Rotation.Val = y;
            axisZ.Rotation.Val = z;
        }

        public void setValues(Telemetria values)
        {
            speed   = values.Speed;
            level   = values.Level;

            axisX   = values.AxisX;
            axisY   = values.AxisY;
            axisZ   = values.AxisZ;
        }

        public override string ToString()
        {
            return string.Format("Lat:{0} Lng:{1}", latitude, longitude) + " " + axisX.ToString() + " " + axisY.ToString() + " " + axisZ.ToString();
        }

        public void setSpeed(int v)
        {
            speed.Val = v;
        }

        public void setLevel(int v)
        {
            level.Val = v;
        }

        public void setInsideOfFence(int index,bool status)
        {
            insideOfFence[index] = status;
        }

        public bool IsInsideOfFence()
        {
            bool ret = false;

            if (insideOfFence != null) {

                foreach (KeyValuePair<int, bool> entry in insideOfFence)
                {
                    if (entry.Value)
                    {
                        ret = true;
                        break;
                    }
                }
            }

            return ret;
        }
    }
}