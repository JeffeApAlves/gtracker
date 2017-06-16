using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    class DecoderFrameRx : IDecoderFrameRx
    {
        const char CHAR_SEPARATOR = ':';

        string formatCMD(string frame)
        {
            string ret      = frame;

            /*            char delim[]    = { CHAR_SEPARATOR, 0 };

                        if (strstr(rxFrame.data, delim) == null)
                        {
                            char size_cmd = 0;

                            while (size_cmd < rxFrame.count && (rxFrame.data[size_cmd] < '0' || rxFrame.data[size_cmd] > '9'))
                            {
                                size_cmd++;
                            };

                            if (rxFrame.getCount() >= (size_cmd + 1))
                            {
                                str_append(rxFrame.data, delim, size_cmd);

                                if ((size_cmd + LEN_ADDRESS + 1) < strlen(rxFrame.data))
                                {

                                    str_append(rxFrame.data, delim, size_cmd + LEN_ADDRESS + 1);
                                }
                            }
                        }*/

            return ret;
        }

        public bool FillCmd(out ParamCmd param, RxFrame frame)
        {
            string frame_format = formatCMD(frame.getPayLoad());

            string[] list       = frame_format.Split(CHAR_SEPARATOR);

            bool ret = false;

            param = new ParamCmd();

            if (list != null)
            {
                        /*
                for (int i = 0; i < list.Length; i++)
                {
                    if (list[i] != null)
                    {
                        switch (i)
                        {
                            case 0: strcpy(param.Name_cmd, list.itens[0]); break;
                            case 1: param.Address = atoi(list.itens[1]); break;
                            case 2: param.Value = atoi(list.itens[2]); break;
                        }
                    }
                }
                */
                ret = list.Length >= 2;
            }

            return ret;
        }
    }
}


/*
     public static void Main()
   {
      StringBuilder sb = new StringBuilder();
      bool flag = true;
      string[] spellings = { "recieve", "receeve", "receive" };
      sb.AppendFormat("Which of the following spellings is {0}:", flag);
      sb.AppendLine();
      for (int ctr = 0; ctr <= spellings.GetUpperBound(0); ctr++) {
         sb.AppendFormat("   {0}. {1}", ctr, spellings[ctr]);
         sb.AppendLine();
      }
      sb.AppendLine();
      Console.WriteLine(sb.ToString());
   }
 */
