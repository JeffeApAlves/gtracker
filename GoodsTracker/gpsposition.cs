using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class GPSPosition
    {
        double latitude, longitude;

        public double Latitude { get => latitude; set => latitude = value; }
        public double Longitude { get => longitude; set => longitude = value; }

        internal GPSPosition(double lat,double lng)
        {
            latitude = lat;
            longitude = lng;
        }

        internal GPSPosition()
        {
            latitude = 0;
            longitude = 0;
        }

        public override string ToString()
        {
            return string.Format("Lat:{0} Lng:{1}", latitude, longitude);
        }

    }
}
