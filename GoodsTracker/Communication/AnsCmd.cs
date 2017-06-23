namespace GoodsTracker
{
    internal class AnsCmd
    {
        Header header;

        TelemetriaData info;

        internal TelemetriaData Info { get => info; set => info = value; }
        internal Header Header { get => header; set => header = value; }

        internal AnsCmd()
        {
            header  = new Header();
            info    = null;
        }

        internal AnsCmd(string r,Operation o)
        {
            header = new Header();

            header.Resource = r;
            header.Operation = o;
            info = null;
        }
    }
}