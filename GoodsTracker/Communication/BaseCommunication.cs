using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace GoodsTracker
{
    internal delegate ResultExec onAnswerCmd(AnsCmd ans);

    public enum ResultExec
    {
        EXEC_UNSUCCESS  = -3,
        INVALID_CMD     = -2,
        INVALID_PARAM   = -1,
        EXEC_SUCCESS    = 0,
    };

    abstract class BaseCommunication
    {
        /*
         * Endereco da unidade
         */
        protected int   address;
        internal int    Address { get => address; set => address = value; }
        private static  Dictionary<string, onAnswerCmd> answerHandler = new Dictionary<string, onAnswerCmd>();

        protected abstract void onReceiveAnswer(AnsCmd ans);

        internal BaseCommunication(int val)
        {
            address = val;
            Communication.AddUnit(this);
        }

        internal Cmd createCMD(int dest, Operation o, string resource)
        {
            Cmd c               = new Cmd(resource, o);
            c.Header.Dest       = dest;
            c.Header.Address    = Master.ADDRESS;

            return c;
        }

        internal void sendCMD(Cmd cmd, onAnswerCmd handler)
        {
            answerHandler[cmd.Header.Resource] = handler;

            Communication.addCmd(cmd);
        }

        internal void sendCMD(Cmd cmd)
        {
            Communication.addCmd(cmd);
        }

        /**
         * 
         * Metodo invocado quando se recebe qualquer resposta
         * 
         */
        internal void processAnswer(Cmd cmd,AnsCmd ans)
        {
            try
            {
                // Executa callback de recebimento de resposta de comando
                onReceiveAnswer(ans);

                // Executar callback respectiva do tipo de comando
                if (answerHandler.ContainsKey(ans.Header.Resource))
                {
                    answerHandler[ans.Header.Resource]?.Invoke(ans);
                }

                // Executa callback do comando
                if (cmd != null){

                    // Especifico do comando
                    cmd.EventAnswerCmd();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Erro na execucao das callbacks de communicacao");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }
        }
    }
}
