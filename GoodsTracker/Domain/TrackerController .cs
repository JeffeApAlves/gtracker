using System.Collections.Generic;

namespace GoodsTracker
{
    public delegate void onUpdateTelemetria(TelemetriaData telemetria);

    class TrackerController :ThreadRun
    {
        const int NUM_ESTACAO = 1;
        const int TIME_TELEMETRIA_MS = 1000;

        onUpdateTelemetria  onDataTelemetria;

        private static TrackerController singleton=null;

        Tracker             tracker;
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

        internal Tracker Tracker { get => tracker; set => tracker = value; }
        public onUpdateTelemetria OnDataTelemetria { get => onDataTelemetria; set => onDataTelemetria = value; }

        private TrackerController()
        {
            tracker     = new Tracker(NUM_ESTACAO);
            fences      = new List<Fence>();
            routes      = new List<Route>();

            setTime(TIME_TELEMETRIA_MS);
        }

        /*
         * 
         * Metodo chamdo na Thread do Domain
         * 
         */
        public override void run()
        {
            requestBehavior();
        }

        internal Fence createFence()
        {
            Fence fence = new Fence();

            return fence;
        }

        internal Route createRoute(string name)
        {
            Route route = new Route(name);

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

        internal TelemetriaData[] getBehaviorFiltered(int i)
        {
            TelemetriaData[] ret = null;

            if (anyRoute())
            {
                ret = routes[0].getBehaviorFiltered(i).ToArray();

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

        internal void requestBehavior()
        {
            tracker.requestBehavior(onReceiveBehavior);
        }

        internal void unLockVehicle()
        {
            tracker.unLockVehicle(onLockStatus);
        }

        internal void lockVehicle()
        {
            tracker.lockVehicle(onLockStatus);
        }

        internal void registerBehavior(TelemetriaData b)
        {
            if (anyRoute() && b != null)
            {
                routes[0].registerBehavior(b);
            }
        }

        internal bool anyRoute()
        {
            return routes.Count > 0 && routes[0].MapRoute != null && routes[0].MapRoute.Points.Count>0;
        }

        internal int getCountRegisters()
        {
            return anyRoute() ? routes[0].Behaviors.Count : 0;
        }

        public TelemetriaData getTelemetria()
        {
            return Tracker.getTelemetria();
        }

        // -------------------------  CallBacks CMDs -------------------------------------------

        private ResultExec onLockStatus(AnsCmd ans)
        {
            return ResultExec.EXEC_SUCCESS;
        }

        private ResultExec onReceiveBehavior(AnsCmd ans)
        {
            //`Atualiza entidade
            tracker.TelemetriaData = ans.Info;

            // Registra a telemetria
            registerBehavior(getTelemetria());

            // Chama alguma call back para notificacao do front End
            onDataTelemetria?.Invoke(getTelemetria());

            return ResultExec.EXEC_SUCCESS;
        }
    }
}
