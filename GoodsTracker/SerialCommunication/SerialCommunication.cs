using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class SerialCommunication : Communication
    {
        static Protocol channel = null;
        static Serial com_port = null;

        public override void Init()
        {
            base.Init();

            com_port = new Serial();
            com_port.Open();

            if (channel == null)
            {
                channel = new Protocol(com_port);
            }

            channel.acceptAnswer = acceptAnswer;

            start();
        }

        public override void DeInit()
        {
            base.DeInit();
            com_port.Close();
        }

        public override void send(DataFrame frame)
        {
            channel.sendFrame(frame);
        }

        public override bool send(Cmd cmd)
        {
            bool flag = false;
            try
            {
                DataFrame frame = new DataFrame(cmd.Header, cmd.Payload);
                channel.writeTx(frame);
                printTx("TX",frame);

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
            channel.receive();
 
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
            processAnswer(cmd,ans);
            removeTxCmd(cmd);
            removeAns(ans);
        };
    }
}
