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

    /*
     * Classe base para comunicacao 
     */
    abstract class BaseCommunication
    {
        /*
         * Endereco da unidade
         */
        protected int   address;
        private static Dictionary<string, onAnswerCmd> answerHandler = new Dictionary<string, onAnswerCmd>();
        private Communic communic;
        protected abstract void onReceiveAnswer(AnsCmd ans);

        internal int    Address { get => address; set => address = value; }
        public Communic Communic { get => communic; set => communic = value; }


        /**
         * Construtor 
         * 
         */
        internal BaseCommunication(int val)
        {
            address = val;

            Communication.registerDevice(this);
        }

        /*
         * Cria cmd's se associado ao endereço do periferico 
         * 
         */
        internal Cmd createCMD(int address,int dest, Operation o, string resource)
        {
            Cmd c               = new Cmd(resource, o);
            c.Header.Dest       = dest;
            c.Header.Address    = address;

            return c;
        }


        /*
         * Cria ans's e associa ao endereço do periferico 
         * 
         */
        internal AnsCmd createAnsCmd(int address, int dest,string resource)
        {
            AnsCmd ans = new AnsCmd(resource, Operation.AN);

            ans.Header.Address = address;
            ans.Header.Dest = dest;
 
            return ans;
        }

         /**
         * 
         * Metodo invocado quando se recebe qualquer resposta para o endereço
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
                if (cmd != null)
                {
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

        /**
          * Envia um comando colocando ele na FIFO de saida que sera processada na Thread de comunicação
          */
        internal void sendCMD(Cmd cmd, onAnswerCmd handler)
        {
            answerHandler[cmd.Header.Resource] = handler;

            communic.send(cmd);
        }

        /**
         * Envia um comando colocando ele na FIFO de saida que sera processada na Thread de comunicação
         */
        internal void sendCMD(Cmd cmd)
        {
            communic.send(cmd);
        }

        /*
         * Envia telemetria
         * 
         */
        internal void sendTLM(AnsCmd ans)
        {
            communic.send(ans);
        }
    }
}
