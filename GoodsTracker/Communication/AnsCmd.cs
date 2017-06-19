namespace GoodsTracker
{
    public struct ObjectValueRXAxis
    {
        public double acceleration;
        public double rotation;
    };

    public struct ObjectValueRX
    {
        public int orig;
        public int dest;
        public string operation;
        public int resource;
        public int size;
        public double latitude;
        public double longitude;
        public ObjectValueRXAxis X, Y, Z;
        public double level;
        public int checksum;
        public int speed;
    };

    internal delegate ResultExec CallBackAnsCmd(AnsCmd dados);

    internal class AnsCmd
    {
        string resource;

        ObjectValueRX dadosRx;

        public ObjectValueRX DadosRx { get => dadosRx; set => dadosRx = value; }
        public string Resource { get => resource; set => resource = value; }
    }
}