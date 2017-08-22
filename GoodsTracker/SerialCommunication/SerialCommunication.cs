using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class SerialCommunication : Communication
    {
 
        static Protocol serialProtocol = null;
        static Serial serialPort = null;

        public override void Init()
        {
            base.Init();

            serialPort = new Serial();
            serialPort.Open();

            if (serialProtocol == null)
            {
                serialProtocol = new Protocol(serialPort);
            }

            serialProtocol.acceptAnswer = acceptAnswer;

            start();
        }

        public override void DeInit()
        {
            base.DeInit();
            serialPort.Close();
        }

        public override void send(DataFrame frame)
        {
            serialProtocol.sendFrame(frame);
        }

        public override bool send(Cmd cmd)
        {
            bool flag = false;
            try
            {
                DataFrame frame = new DataFrame(cmd.Header, cmd.Payload);
                serialProtocol.writeTx(frame);
                printTx("TX OK: ",frame);

                flag = true;
            }catch(Exception e)
            {
                Console.WriteLine(e);
                flag = false;
            }

            return flag;
        }

        public override bool receive()
        {
            serialProtocol.receive();
 
            return true;
        }

        /*
         * Callback chamada quando um frame é recebido com sucesso
         * 
         **/
        acceptAnswerHandler acceptAnswer = (AnsCmd ans) =>
        {
            Cmd cmd = searchCmdOfAnswer(ans);

            addAns(ans);
            processAnswer(ans);
            removeTxCmd(cmd);
            removeAns(ans);
        };
    }
}
