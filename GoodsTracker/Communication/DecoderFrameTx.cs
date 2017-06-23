using System;
using System.Text;

namespace GoodsTracker
{
    enum DATA_INDEX
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

        ROT_X       = 10,
        ROT_Y       = 11,
        ROT_Z       = 12,

        SPEED       = 13,
        LEVEL       = 14,
        DATETIME    = 15,
    }

    internal class DecoderFrame : IDecoderFrameTx,IDecoderFrameRx
    {
        public bool setHeader(ref CommunicationFrame frame, Cmd cmd)
        {
            bool ret = false;

            frame = new CommunicationFrame();

            try
            {
                StringBuilder sb = new StringBuilder();

                //Header
                sb.Append(cmd.Address.ToString("D5"));
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

        public bool setHeader(ref CommunicationFrame frame, AnsCmd ans)
        {
            bool ret = false;

            frame = new CommunicationFrame();

            try
            {
                StringBuilder sb = new StringBuilder();

                //Header
                sb.Append(ans.Address.ToString("D5"));
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(ans.Dest.ToString("D5"));
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(ans.Operation);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(ans.Resource);
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
                sb.Append(b.AxisX.Rotation.Val);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.AxisY.Rotation.Val);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(b.AxisZ.Rotation.Val);

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
                    ans.Address        = getValAsInteger(list, DATA_INDEX.ORIG);
                    ans.Dest        = getValAsInteger(list, DATA_INDEX.DEST);
                    ans.Operation   = getValAsOperation(list, DATA_INDEX.OPERACAO);
                    ans.Resource    = getValAsString(list, DATA_INDEX.RESOURCE);
                    ans.Size        = getValAsInteger(list, DATA_INDEX.SIZE_PAYLOAD);

                    dadosRx.setPosition(getValAsDouble(list, DATA_INDEX.LAT), 
                                        getValAsDouble(list, DATA_INDEX.LNG));

                    dadosRx.setAcceleration(getValAsDouble(list, DATA_INDEX.ACCEL_X),
                                            getValAsDouble(list, DATA_INDEX.ACCEL_Y),
                                            getValAsDouble(list, DATA_INDEX.ACCEL_Z));

                    dadosRx.setRotation(    getValAsDouble(list, DATA_INDEX.ROT_X),
                                            getValAsDouble(list, DATA_INDEX.ROT_Y),
                                            getValAsDouble(list, DATA_INDEX.ROT_Z));

                    dadosRx.Speed.Val   = getValAsDouble(list, DATA_INDEX.SPEED);
                    dadosRx.Level.Val   = getValAsDouble(list, DATA_INDEX.LEVEL);

                    dadosRx.DateTime    = getValAsDateTime(list, DATA_INDEX.DATETIME);

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

        private Operation getValAsOperation(string[] list, DATA_INDEX oPERACAO)
        {
            string str = getValAsString(list, oPERACAO);

            return (Operation)Enum.Parse(typeof(Operation), str);
        }

        private DateTime getValAsDateTime(string[] list, DATA_INDEX index)
        {
            DateTime d = Convert.ToDateTime("01/01/1900");

            if ((int)index < list.Length)
            {
                d = Convert.ToDateTime(list[(int)index].Replace('.', ':'));
            }
            return d;
        }

        private int getValAsInteger(string[] list, DATA_INDEX index)
        {
            int dest = 0;

            if ((int)index < list.Length)
            {
                dest = Convert.ToInt16(list[(int)index]);
            }

            return dest;
        }

        private double getValAsDouble(string[] list, DATA_INDEX index)
        {
            double dest = 0;

            if ((int)index < list.Length)
            {
                dest = Convert.ToDouble(list[(int)index]);
            }

            return dest;
        }

        private string getValAsString(string[] list, DATA_INDEX index)
        {
            string dest = "";

            if ((int)index < list.Length)
            {
                dest = list[(int)index];
            }

            return dest;
        }
    }
}