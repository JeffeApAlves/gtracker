using GMap.NET;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace GoodsTracker
{
    class LayerMap : Object
    {
        bool Visible = true;
        static GMapControl mapControl = null;
        GMapOverlay mapOverlay = null;
        List<string> listFecence = new List<string>();

        internal LayerMap(GMapControl mc,string name)
        {
            if (mapOverlay == null)
            {
                mapControl = mc;
                mapOverlay = new GMapOverlay(name);
                mapControl.Overlays.Add(mapOverlay);
                Visible = true;
            }
        }

        internal void addPosition(PointLatLng position)
        {
            GMarkerGoogle marker;

            marker = new GMarkerGoogle(position, GMarkerGoogleType.green);
            marker.ToolTipMode = MarkerTooltipMode.Always;
            marker.ToolTipText = string.Format("Localizacao\n Latitude:{0} \n Longitude:{1}", position.Lat, position.Lng);

            mapOverlay.Markers.Add(marker);
        }

        internal void addPosition(double lat, double lng)
        {
            addPosition(new PointLatLng(lat, lng));
        }

        internal void removePositionAt(int item)
        {
            if (item >= 0)
            {
                mapOverlay.Markers.RemoveAt(item);
            }
        }

        internal void addFence(Fence fence)
        {
            GMapPolygon polygon;

            polygon = new GMapPolygon(fence.getPoints(), "Fence");
            polygon.Fill = new SolidBrush(Color.FromArgb(50, Color.Red));
            polygon.Stroke = new Pen(Color.Red, 1);

            mapOverlay.Polygons.Add(polygon);

            listFecence.Add(string.Format("Fence:{0}",listFecence.Count+1));
        }

        internal void addFence(DataTable dt)
        {
            Fence fence = new Fence();

            fence.setFence(dt);

            addFence(fence);
        }

        internal void show()
        {
            if (!Visible)
            {
                Visible = true;
                mapControl.Overlays.Add(mapOverlay);
                mapControl.Refresh();
            }
        }

        internal void hide()
        {
            if (Visible)
            {
                Visible = false;
                mapControl.Overlays.Remove(mapOverlay);
                mapControl.Refresh();
            }
        }

        internal void show(bool flg)
        {
            if (flg) {
                show();
            }
            else
            {
                hide();
            }
        }

        internal bool isVisible()
        {
            return Visible;
        }

        internal void removeFenceAt(int index)
        {
            if (index >= 0 && index < mapOverlay.Polygons.Count)
            {
                mapOverlay.Polygons.RemoveAt(index);
            }
        }

        internal List<string> getListFence()
        {
            return listFecence;
        }
    }
}
