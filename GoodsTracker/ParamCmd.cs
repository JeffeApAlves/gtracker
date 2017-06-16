using System;

namespace GoodsTracker
{
    internal class ParamCmd
    {
        private string  name_cmd;
        private uint address;
        private uint value;

        public ParamCmd()
        {
            clear();
        }

        public string NameCmd { get => name_cmd; set => name_cmd = value; }
        public uint Address { get => address; set => address = value; }
        public uint Value { get => value; set => this.value = value; }

        internal void clear()
        {
            NameCmd     = "";
            Address     = 0;
            Value       = 0;
        }
    }
}