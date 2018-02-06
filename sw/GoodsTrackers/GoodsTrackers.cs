using System.Diagnostics;

namespace GoodsTracker
{
    class GTracker
    {
        private static Communic communic = null;

        internal static Communic Communic { get => communic; set => communic = value; }

        public static void createCommunication(TYPE_COMMUNICATION type)
        {
            if (Communication.Type != type || communic == null)
            {
                if (communic != null)
                {
                    communic.Stop();
                }

                communic = Communication.create(type);

                if (communic != null)
                {
                    communic.Start();
                }
                else
                {
                    Debug.WriteLine("Problema na inicializacao da comunicação");
                }
            }
        }

        public static void stopCommunication()
        {
            if (communic != null)
            {
                Communic.Stop();
            }
        }
    }
}
