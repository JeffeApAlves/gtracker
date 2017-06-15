using GMap.NET;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace GoodsTracker
{
    class LayerMap : Object
    {
        bool Visible = true;
        GMapControl mapControl = null;
        GMapOverlay mapOverlay = null;

        public LayerMap(GMapControl mc,string name)
        {
            if (mapOverlay == null)
            {
                mapControl = mc;
                mapOverlay = new GMapOverlay(name);
                mapControl.Overlays.Add(mapOverlay);
                Visible = true;
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

        public void show()
        {
            if (!Visible)
            {
                Visible = true;
                mapControl.Overlays.Add(mapOverlay);
                mapControl.Refresh();
            }
        }

        public void hide()
        {
            if (Visible)
            {
                Visible = false;
                mapControl.Overlays.Remove(mapOverlay);
                mapControl.Refresh();
            }
        }

        public void show(bool flg)
        {
            if (flg) {
                show();
            }
            else
            {
                hide();
            }
        }

        public bool isVisible()
        {
            return Visible;
        }

    }
}
