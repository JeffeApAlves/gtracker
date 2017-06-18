using System.Collections.Generic;

namespace GoodsTracker
{
    class TrackerController
    {
        private static TrackerController singleton=null;

        Tracker         tracker;
        List<Fence>     fences;
        List<Route>     routes;

        internal List<Fence> Fences { get => fences; set => fences = value; }
        internal List<Route> Routes { get => routes; set => routes = value; }

        //Singleton
        public static TrackerController TrackerCtrl
        {
            get
            {
                if (singleton == null)
                {
                    singleton = new TrackerController();
                }

                return singleton;
            }
        }

        private TrackerController()
        {
            tracker     = new Tracker();
            fences      = new List<Fence>();
            routes      = new List<Route>();
        }

        public void process()
        {

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
