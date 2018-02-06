using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace GoodsTracker
{
    delegate void onUpdateTelemetria(Telemetria telemetria);

    class TrackerController :ThreadRun
    {

        private static TrackerController singleton = null;

        private const int _TIME_TELEMETRIA  = 100;

        onUpdateTelemetria  onDataTelemetria;

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

        public static int TIME_TELEMETRIA => _TIME_TELEMETRIA;

        private TrackerController()
        {
            tracker     = new Tracker(2);
            fences      = new List<Fence>();
            routes      = new List<Route>();

            setTime(_TIME_TELEMETRIA);
        }

        /*
         * 
         * Metodo chamado na Thread do Domain
         * 
         */
        public override void run()
        {
            requestBehavior();
        }

        /*
        * 
        * Cria uma cerca
        * 
        */
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

        internal Telemetria[] getBehaviorFiltered(int i)
        {
            Telemetria[] ret = null;

            if (anyRoute())
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

        internal void requestBehavior()
        {
            // Verifica se recebeu nos ultimos 5s uma TLM caso negativo requisita
            if (tracker.getLastUpdate() > 2000)
            {
                tracker.requestBehavior(onTLM);
            }
        }

        internal void lockVehicle(bool flg)
        {
            if (flg != getTelemetria().StatusLock)
            {
                if (flg)
                {
                    tracker.lockVehicle(onLCK);
                }
                else
                {
                    tracker.unLockVehicle(onLCK);
                }
            }
        }

        internal void registerBehavior(Telemetria b)
        {
            if (anyRoute() && b != null)
            {
                routes[0].registerBehavior(b);
            }
        }

        internal void clearBehavior()
        {
            if (anyRoute())
            {
                routes[0].Behaviors.Clear();
            }
        }

        /*
         * 
         * Indica se tem alguma rota
         */ 
        internal bool anyRoute()
        {
            return routes.Count > 0 && routes[0].MapRoute != null && routes[0].MapRoute.Points.Count>0;
        }

        /*
         * 
         * Retorna numero de telemetria registrada
         */
        internal int getCountRegisters()
        {
            return anyRoute() ? routes[0].Behaviors.Count : 0;
        }

        /*
         * Retorna ultima telemetria
         */
        public Telemetria getTelemetria()
        {
            return Tracker.getTelemetria();
        }

        /*
         * Evento chamado quando o rastreador responder por um comando de LCK
         */
        private ResultExec onLCK(AnsCmd ans)
        {
            return ResultExec.EXEC_SUCCESS;
        }

        /*
         * Evento chamado quando o rastreador responder por um comando de TLM
         */
        private ResultExec onTLM(AnsCmd ans)
        {
            // Registra a telemetria
            registerBehavior(getTelemetria());

            // Chama alguma call back para notificacao do front End
            onDataTelemetria?.Invoke(getTelemetria());

            return ResultExec.EXEC_SUCCESS;
        }

        public void Init()
        {
            // Inicia a Thread
            start();
        }
    }
}
