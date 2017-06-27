using GMap.NET;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System;
using System.Drawing;

namespace GoodsTracker
{
    class LayerMap : Object
    {
        static GMapControl  mapControl  = null;
        bool                Visible     = true;
        GMapOverlay         mapOverlay  = null;

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

        internal void add(PointLatLng position)
        {
            string str = string.Format("Lat.:{0} \n Lng.:{1}", position.Lat, position.Lng);

            add(position, str, GMarkerGoogleType.blue);
        }


        internal void add(PointLatLng position,string str, GMarkerGoogleType color)
        {
            GMarkerGoogle marker;

            marker = new GMarkerGoogle(position, color);
            marker.ToolTipMode = MarkerTooltipMode.Always;
            marker.ToolTipText = string.Format(str);

            mapOverlay.Markers.Add(marker);
        }

        internal void add(PointLatLng position, GMarkerGoogleType color)
        {
            add(position, "", color);
        }

        internal void add(double lat, double lng)
        {
            add(new PointLatLng(lat, lng));
        }

        internal void add(string name,Fence fence)
        {
            GMapPolygon     polygon;

            polygon         = new GMapPolygon(fence.Points, name);
            polygon.Fill    = new SolidBrush(Color.FromArgb(50, Color.Red));
            polygon.Stroke  = new Pen(Color.Red, 1);

            mapOverlay.Polygons.Add(polygon);
        }

        internal void add(GMapRoute route)
        {
            mapOverlay.Routes.Add(route);
        }

        internal void add(Route route)
        {
            add(route.MapRoute);
        }

        internal void removeFenceAt(int index)
        {
            if (index >= 0 && index < mapOverlay.Polygons.Count)
            {
                removeMarkersOfFence(mapOverlay.Polygons[index]);

                mapOverlay.Polygons.RemoveAt(index);
            }
        }

        internal void removeMarkersOfFence(GMapPolygon polygon)
        {
            foreach (PointLatLng point in polygon.Points)
            {
                foreach (GMapMarker marker in mapOverlay.Markers)
                {
                    if (marker.Position.Equals(point))
                    {
                        mapOverlay.Markers.Remove(marker);
                        break;
                    }
                }
            }
        }

        internal void removeMarkerAt(int index)
        {
            if (index >= 0 && index < mapOverlay.Markers.Count)
            {
                mapOverlay.Markers.RemoveAt(index);
            }
        }

        internal void removeAllMarkers()
        {
            mapOverlay.Markers.Clear();
        }

        internal void removeRouteAt(int index)
        {
            if (index >= 0 && index < mapOverlay.Markers.Count)
            {
                mapOverlay.Routes.RemoveAt(index);
            }
        }

        internal void remove(Route route)
        {
            if (route!=null && route.MapRoute!=null)
            {
                mapOverlay.Routes.Remove(route.MapRoute);
                removeAllMarkers();
            }
        }

        internal bool PointIsInsidePolygon(TelemetriaData data)
        {
            bool ret = false;

            if (data != null)
            {
                PointLatLng p = new PointLatLng(data.Latitude, data.Longitude);

                int i = 0;

                foreach (GMapPolygon poly in mapOverlay.Polygons)
                {
                    data.setInsideOfFence(i++, poly.IsInside(p));

                    if (data.IsInsideOfFence())
                    {
                        ret = true;
                    }
                }
            }

            return ret;
        }
    }
}
