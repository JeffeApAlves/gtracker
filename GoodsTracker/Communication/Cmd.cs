using System;

namespace GoodsTracker
{
    class Cmd
    {
        onAnswerCmd onAnswerCmd;
        Header      header;
        PayLoad     payload;
        
        public onAnswerCmd EventAnswerCmd { get => onAnswerCmd; set => onAnswerCmd = value; }
        internal Header Header { get => header; set => header = value; }
        internal PayLoad Payload { get => payload; set => payload = value; }

        internal Cmd(string r,Operation o)
        {
            header = new Header();
            payload = new PayLoad();

            header.Resource = r;
            header.Operation = o;
        }

        internal void Append(string str)
        {
            payload.Append(str);
        }
    }
}
