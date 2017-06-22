using System;
using System.Text;

namespace GoodsTracker
{
    // [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[3] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ] \r\n

    enum DATA1
    {
        ORIG        = 0,
        DEST        = 1,
        OPERACAO    = 2,
        RESOURCE    = 3,
        SIZE_PAYLOAD= 4,
        LAT         = 5,
        LNG         = 6,
        ACCEL_X     = 7,
        ACCEL_Y     = 8,
        ACCEL_Z     = 9,
        SPEED       = 10,
        LEVEL       = 11,
        DATETIME    = 12,
    }

    internal class DecoderFrame : IDecoderFrameTx,IDecoderFrameRx
    {
        public bool setHeader(ref CommunicationFrame frame, CommunicationUnit unit)
        {
            bool ret = false;

            frame = new CommunicationFrame();

            try
            {
                Cmd cmd = unit.getNextCmd();

                setHeader(ref frame, unit, cmd);

                ret = true;
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        public bool setHeader(ref CommunicationFrame frame, CommunicationUnit unit,Cmd cmd)
        {
            bool ret = false;

            frame = new CommunicationFrame();

            try
            {
                StringBuilder sb = new StringBuilder();

                //Header
                sb.Append(unit.Address.ToString("D5"));
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(cmd.Dest.ToString("D5"));
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(cmd.Operation);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(cmd.Resource);
                sb.Append(CONST_CHAR.SEPARATOR);
                frame.Header = sb.ToString();

                ret = true;
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        public bool setPayLoad(ref CommunicationFrame frame, TelemetriaData b)
        {
            bool ret = false;

            try
            {
                StringBuilder sb = new StringBuilder();

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.Latitude);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.Longitude);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.AxisX.Acceleration.Val);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.AxisY.Acceleration.Val);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.AxisZ.Acceleration.Val);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.Speed.Val);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.Level.Val);

                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.DateTime.ToString().Replace(':','.'));

                frame.PayLoad = sb.Length.ToString("D3")+sb.ToString();

                ret = true;
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        public bool getValues(out AnsCmd ans, CommunicationFrame frame)
        {
            bool ret = false;
            ans                     = new AnsCmd();
            TelemetriaData  dadosRx = new TelemetriaData();

            try
            {
                string[] list = frame.Frame.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 9)
                {
                    ans.Orig        = getValAsInteger(list, (int)DATA1.ORIG);
                    ans.Dest        = getValAsInteger(list, (int)DATA1.DEST);
                    ans.Operation   = getValAsString(list, (int)DATA1.OPERACAO);
                    ans.Resource    = getValAsString(list, (int)DATA1.RESOURCE);
                    ans.Size        = getValAsInteger(list, (int)DATA1.SIZE_PAYLOAD);

                    dadosRx.setPosition(getValAsDouble(list, (int)DATA1.LAT), 
                                        getValAsDouble(list, (int)DATA1.LNG));

                    dadosRx.setAcceleration(getValAsDouble(list, (int)DATA1.ACCEL_X),
                                            getValAsDouble(list, (int)DATA1.ACCEL_Y),
                                            getValAsDouble(list, (int)DATA1.ACCEL_Z));

                    dadosRx.Speed.Val   = getValAsDouble(list, (int)DATA1.SPEED);
                    dadosRx.Level.Val   = getValAsDouble(list, (int)DATA1.LEVEL);

                    dadosRx.DateTime    = getValAsDateTime(list, (int)DATA1.DATETIME);

                    ans.Info = dadosRx;

                    ret = true;
                }
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        private DateTime getValAsDateTime(string[] list, int index)
        {
            DateTime d = Convert.ToDateTime("01/01/1900");

            if (index < list.Length)
            {
                d = Convert.ToDateTime(list[index].Replace('.', ':'));
            }
            return d;
        }

        private int getValAsInteger(string[] list, int index)
        {
            int dest = 0;

            if (index < list.Length)
            {

                dest = Convert.ToInt16(list[index]);
            }

            return dest;
        }


        private double getValAsDouble(string[] list, int index)
        {
            double dest = 0;

            if (index < list.Length)
            {

                dest = Convert.ToDouble(list[index]);
            }

            return dest;
        }

        private string getValAsString(string[] list, int index)
        {
            string dest = "";

            if (index < list.Length)
            {

                dest = list[index];
            }

            return dest;
        }


        private void setVal(ref double dest, string[] list, int index)
        {
            dest = 0;

            if (index < list.Length)
            {

                dest = Convert.ToDouble(list[index]);
            }
        }

        private void setVal(ref int dest, string[] list, int index)
        {
            dest = 0;

            if (index < list.Length)
            {
                dest = Convert.ToInt16(list[index]);
            }
        }

        private void setVal(ref string dest, string[] list, int index)
        {
            dest = "";

            if (index < list.Length)
            {
                dest = list[index];
            }
        }

    }
}