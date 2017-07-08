using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace GoodsTracker
{
    public enum ResultExec
    {
        EXEC_UNSUCCESS  = -3,
        INVALID_CMD     = -2,
        INVALID_PARAM   = -1,
        EXEC_SUCCESS    = 0,
    };

    internal delegate ResultExec onAnswerCmd(AnsCmd ans);

    abstract class CommunicationUnit
    {
        /*
         * Endereco da unidade
         */
        protected int   address;
        internal int    Address { get => address; set => address = value; }

        protected abstract void onReceiveAnswer(AnsCmd ans);

        internal CommunicationUnit(int val)
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

        internal void sendCMD(Cmd cmd)
        {
            Communication.addCmd(cmd);
        }

        internal void processAnswer(AnsCmd ans)
        {
            try
            {
                // Pega o comando respectivo
                Cmd cmd = Communication.searchCmdOfAnswer(ans);

                if (cmd != null)
                {
                    // Executa evento de recebmento de resposta de comando
                    onReceiveAnswer(ans);

                    // Executa call back respectiva do comando
                    cmd.EventAnswerCmd?.Invoke(ans);
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
