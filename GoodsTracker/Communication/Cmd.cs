using System;

namespace GoodsTracker
{
    class Cmd
    {
        Header      header;
        PayLoad     payload;
        
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

        /**
         * 
         * Evento chamado quando recebe a resposta respectiva ao comando
         * 
         */
        public void EventAnswerCmd()
        {
            //TODO se necessario
        }
    }
}
