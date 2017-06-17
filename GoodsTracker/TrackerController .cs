using System.Collections.Generic;

namespace GoodsTracker
{
    class TrackerController
    {
        Tracker         tracker;
        List<Fence>     fences;
        List<Route>     routes;

        internal List<Fence> Fences { get => fences; set => fences = value; }
        internal List<Route> Routes { get => routes; set => routes = value; }

        internal TrackerController()
        {
            tracker     = new Tracker();
            fences      = new List<Fence>();
            routes      = new List<Route>();
        }

        internal Fence createFence()
        {
            Fence fence = new Fence();

            return fence;
        }

        internal Route createRoute()
        {
            Route route = new Route("");

            return route;
        }

        internal void addFence(Fence fence)
        {
            fences.Add(fence);
        }

        internal void removeFenceAt(int index)
        {
            fences.RemoveAt(index);
        }

        internal List<Behavior> getBehaviorFiltered(int i)
        {
            List<Behavior> ret = null;

            if (routes.Count > 0)
            {
                switch (i)
                {
                    case 0: ret = routes[0].Behaviors; break;
                    case 1: ret = routes[0].getItensOK(); break;
                    case 2: ret = routes[0].getItensNOK(); break;
                }
            }

            return ret;
        }

        internal void addRoute(Route r)
        {
            routes.Add(r);
        }

        internal void removeRouteAt(int index)
        {
            routes.RemoveAt(index);
        }

        internal void remove(Route r)
        {
            routes.Remove(r);
        }
    }
}
