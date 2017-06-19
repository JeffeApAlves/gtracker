using System.Collections.Generic;

namespace GoodsTracker
{
    class TrackerController :ThreadRun
    {
        private static TrackerController singleton=null;

        InterfaceTracker    tracker;
        List<Fence>         fences;
        List<Route>         routes;

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
            tracker     = new TrackerTest();
            fences      = new List<Fence>();
            routes      = new List<Route>();
        }

        /*
         * 
         * Metodo chamdo na Thread do Domain
         * 
         */
        public override void run()
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
                ret = routes[0].getBehaviorFiltered(i);
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

        internal void registerBehavior(Behavior b)
        {
            if (routes.Count > 0 && b!=null)
            {
                routes[0].registerBehavior(b);
            }
        }

        public void updateTracker()
        {
            registerBehavior(tracker.getPosition());
        }        
    }
}
