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
        CHECKSUM    = 16,
    }

    internal class DecoderFrame : IDecoderFrameTx,IDecoderFrameRx
    {
        public bool setHeader(ref CommunicationFrame frame, Cmd cmd)
        {
            bool ret = false;

            frame = new CommunicationFrame();

            try
            {
                // Header
                frame.Append(cmd.Address.ToString("D5")+ CONST_CHAR.SEPARATOR);
                frame.Append(cmd.Dest.ToString("D5")+ CONST_CHAR.SEPARATOR);
                frame.Append(cmd.Operation.ToString()+ CONST_CHAR.SEPARATOR);
                frame.Append(cmd.Resource);

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
                frame.Append(ans.Address.ToString("D5") + CONST_CHAR.SEPARATOR);
                frame.Append(ans.Dest.ToString("D5") + CONST_CHAR.SEPARATOR);
                frame.Append(ans.Operation.ToString() + CONST_CHAR.SEPARATOR);
                frame.Append(ans.Resource);

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
                frame.AppendPayLoad(b.Latitude);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.Longitude);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.AxisX.Acceleration.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.AxisY.Acceleration.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.AxisZ.Acceleration.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.AxisX.Rotation.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.AxisY.Rotation.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.AxisZ.Rotation.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.Speed.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.Level.Val);
                frame.AppendPayLoad(CONST_CHAR.SEPARATOR);
                frame.AppendPayLoad(b.DateTime.ToString().Replace(CONST_CHAR.SEPARATOR, '.'));

                frame.PayLoad = frame.getSizeOfPayLoad().ToString("D3") + CONST_CHAR.SEPARATOR + frame.PayLoad;

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
            ans = new AnsCmd();
            TelemetriaData  telemetria = new TelemetriaData();

            try
            {
                string[] list = frame.Frame.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 9)
                {
                    ans.Address     = getValAsInteger(list, DATA_INDEX.ORIG);
                    ans.Dest        = getValAsInteger(list, DATA_INDEX.DEST);
                    ans.Operation   = getValAsOperation(list, DATA_INDEX.OPERACAO);
                    ans.Resource    = getValAsString(list, DATA_INDEX.RESOURCE);
                    ans.Size        = getValAsInteger(list, DATA_INDEX.SIZE_PAYLOAD);
                    
                    telemetria.setPosition( getValAsDouble(list, DATA_INDEX.LAT), 
                                            getValAsDouble(list, DATA_INDEX.LNG));

                    telemetria.setAcceleration( getValAsDouble(list, DATA_INDEX.ACCEL_X),
                                                getValAsDouble(list, DATA_INDEX.ACCEL_Y),
                                                getValAsDouble(list, DATA_INDEX.ACCEL_Z));

                    telemetria.setRotation( getValAsDouble(list, DATA_INDEX.ROT_X),
                                            getValAsDouble(list, DATA_INDEX.ROT_Y),
                                            getValAsDouble(list, DATA_INDEX.ROT_Z));

                    telemetria.Speed.Val   = getValAsDouble(list, DATA_INDEX.SPEED);
                    telemetria.Level.Val   = getValAsDouble(list, DATA_INDEX.LEVEL);
                    telemetria.DateTime    = getValAsDateTime(list, DATA_INDEX.DATETIME);

                    ans.Info                = telemetria;

                    byte cheksumRx          = (byte)getValAsInteger(list, DATA_INDEX.CHECKSUM);

                    frame.Frame             = frame.Frame.Substring(0, frame.Frame.Length - 4);

                    ret = frame.checkSum()==cheksumRx;
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
                d = Convert.ToDateTime(list[(int)index].Replace('.', CONST_CHAR.SEPARATOR));
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