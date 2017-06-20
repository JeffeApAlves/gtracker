namespace GoodsTracker
{
    internal class AnsCmd
    {
        string resource,operation;
        int orig, dest;
        int size;

        TelemetriaData info;

        public string Resource { get => resource; set => resource = value; }
        public int Orig { get => orig; set => orig = value; }
        public int Dest { get => dest; set => dest = value; }
        public int Size { get => size; set => size = value; }
        public string Operation { get => operation; set => operation = value; }
        internal TelemetriaData Info { get => info; set => info = value; }

        internal AnsCmd()
        {
            resource = "";
            operation = "";
            size = 0;
            info = new TelemetriaData();
        }
    }
}