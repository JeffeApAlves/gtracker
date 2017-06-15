using GMap.NET;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class LayerMap
    {
        GMapControl mapControl = null;
        GMapOverlay mapOverlay = null;

        public LayerMap(GMapControl mc,string name)
        {
            if (mapOverlay == null)
            {
                mapControl = mc;
                mapOverlay = new GMapOverlay(name);
                mapControl.Overlays.Add(mapOverlay);
            }
        }

        public void addPosition(PointLatLng position)
        {
            GMarkerGoogle marker;

            marker = new GMarkerGoogle(position, GMarkerGoogleType.green);
            marker.ToolTipMode = MarkerTooltipMode.Always;
            marker.ToolTipText = string.Format("Localizacao\n Latitude:{0} \n Longitude:{1}", position.Lat, position.Lng);

            mapOverlay.Markers.Add(marker);
        }

        public void addPosition(double lat, double lng)
        {
            addPosition(new PointLatLng(lat, lng));
        }

        public void show()
        {
            mapControl.Visible = true;
        }

        public void removePositionAt(int item)
        {
            if (item >= 0)
            {
                mapOverlay.Markers.RemoveAt(item);
            }
        }

        public void addFence(Fence fence)
        {
            mapOverlay.Polygons.Add(fence.getFence());
        }

        public void addFence(DataTable dt)
        {
            Fence fence = new Fence();

            fence.setFence(dt);

            addFence(fence);
        }


        public void hide()
        {
            mapControl.Visible = false;
        }

    }
}
