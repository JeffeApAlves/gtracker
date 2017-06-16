namespace GoodsTracker
{
    class Scale {

        double min;
        double max;

        public double Min { get => min; set => min = value; }
        public double Max { get => max; set => max = value; }
    }
    class Value{

        double val;
        Scale tol;

        public double Val { get => val; set => val = value; }
        internal Scale Tol { get => tol; set => tol = value; }

        public Value()
        {
            tol = new Scale();
        }
    }

    internal class Behavior
    {
        GPSPosition position;
        Value speed;
        Value accelerationX, accelerationY, accelerationZ;
        Value rotationX, rotationY, rotationZ;

        internal Value Speed { get => speed; set => speed = value; }
        internal Value AccelerationX { get => accelerationX; set => accelerationX = value; }
        internal Value AccelerationY { get => accelerationY; set => accelerationY = value; }
        internal Value AccelerationZ { get => accelerationZ; set => accelerationZ = value; }
        internal GPSPosition Position { get => position; set => position = value; }
        internal Value RotationX { get => rotationX; set => rotationX = value; }
        internal Value RotationY { get => rotationY; set => rotationY = value; }
        internal Value RotationZ { get => rotationZ; set => rotationZ = value; }


        public Behavior()
        {
            position        = new GPSPosition();
            speed           = new Value();
            accelerationX   = new Value();
            accelerationY   = new Value();
            accelerationZ   = new Value();

            rotationX       = new Value();
            rotationY       = new Value();
            rotationZ       = new Value();
        }
    }
}