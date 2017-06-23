namespace GoodsTracker
{
    internal class AnsCmd
    {
        int dest;
        int address;
        Operation operation;
        string resource;

        int size;

        TelemetriaData info;

        public string Resource { get => resource; set => resource = value; }
        public int Dest { get => dest; set => dest = value; }
        public int Size { get => size; set => size = value; }
        internal TelemetriaData Info { get => info; set => info = value; }
        public int Address { get => address; set => address = value; }
        public Operation Operation { get => operation; set => operation = value; }

        internal AnsCmd()
        {
            resource    = "";
            operation   = Operation.NN;
            size        = 0;
            info        = new TelemetriaData();
        }

        internal AnsCmd(string r,Operation o)
        {
            resource = r;
            operation = o;
            size = 0;
            info = new TelemetriaData();
        }

    }
}