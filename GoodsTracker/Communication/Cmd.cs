namespace GoodsTracker
{
    class Cmd
    {
        HeaderFrame header;
        PayLoad     payload;
        
        internal HeaderFrame Header { get => header; set => header = value; }
        internal PayLoad Payload { get => payload; set => payload = value; }

        internal Cmd(string r,Operation o)
        {
            header  = new HeaderFrame();
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
