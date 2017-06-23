using System;

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

    internal class DecoderFrame : IDecoderFrame
    {
        static public bool setHeader(Header header)
        {
            bool ret    = false;

            try
            {
                // Header
                header.Append(header.Address.ToString("D5")+ CONST_CHAR.SEPARATOR);
                header.Append(header.Dest.ToString("D5")+ CONST_CHAR.SEPARATOR);
                header.Append(header.Operation.ToString()+ CONST_CHAR.SEPARATOR);
                header.Append(header.Resource);

                ret = true;
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        public bool setValues(out PayLoad payload, TelemetriaData b)
        {
            bool ret = false;

            payload = new PayLoad();

            try
            {
                payload.Append(b.Latitude);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Longitude);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisX.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisY.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisZ.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisX.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisY.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisZ.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Speed.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Level.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.DateTime.ToString().Replace(CONST_CHAR.SEPARATOR, '.'));

                payload.Data = payload.Length().ToString("D3") + CONST_CHAR.SEPARATOR + payload.Data;

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
            bool ret    = false;
            ans         = new AnsCmd();
            TelemetriaData  telemetria = new TelemetriaData();

            try
            {
                string[] list = frame.Data.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 9)
                {
                    ans.Header.Address     = getAsInteger(list, DATA_INDEX.ORIG);
                    ans.Header.Dest        = getAsInteger(list, DATA_INDEX.DEST);
                    ans.Header.Operation   = getAsOperation(list, DATA_INDEX.OPERACAO);
                    ans.Header.Resource    = getAsString(list, DATA_INDEX.RESOURCE);
                    ans.Header.SizePayLoad = getAsInteger(list, DATA_INDEX.SIZE_PAYLOAD);
                    
                    telemetria.setPosition( getAsDouble(list, DATA_INDEX.LAT), 
                                            getAsDouble(list, DATA_INDEX.LNG));

                    telemetria.setAcceleration( getAsDouble(list, DATA_INDEX.ACCEL_X),
                                                getAsDouble(list, DATA_INDEX.ACCEL_Y),
                                                getAsDouble(list, DATA_INDEX.ACCEL_Z));

                    telemetria.setRotation( getAsDouble(list, DATA_INDEX.ROT_X),
                                            getAsDouble(list, DATA_INDEX.ROT_Y),
                                            getAsDouble(list, DATA_INDEX.ROT_Z));

                    telemetria.Speed.Val   = getAsDouble(list, DATA_INDEX.SPEED);
                    telemetria.Level.Val   = getAsDouble(list, DATA_INDEX.LEVEL);
                    telemetria.DateTime    = getAsDateTime(list, DATA_INDEX.DATETIME);

                    ans.Info                = telemetria;

                    byte cheksumRx          = (byte)getAsInteger(list, DATA_INDEX.CHECKSUM);

                    // Excluir checksum
                    frame.Data             = frame.Data.Substring(0, frame.Data.Length - 4);

                    ret = frame.checkSum()==cheksumRx;
                }
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        private Operation getAsOperation(string[] list, DATA_INDEX oPERACAO)
        {
            string str = getAsString(list, oPERACAO);

            return (Operation)Enum.Parse(typeof(Operation), str);
        }

        private DateTime getAsDateTime(string[] list, DATA_INDEX index)
        {
            DateTime d = Convert.ToDateTime("01/01/1900");

            if ((int)index < list.Length)
            {
                d = Convert.ToDateTime(list[(int)index].Replace('.', CONST_CHAR.SEPARATOR));
            }
            return d;
        }

        private int getAsInteger(string[] list, DATA_INDEX index)
        {
            int dest = 0;

            if ((int)index < list.Length)
            {
                dest = Convert.ToInt16(list[(int)index]);
            }

            return dest;
        }

        private double getAsDouble(string[] list, DATA_INDEX index)
        {
            double dest = 0;

            if ((int)index < list.Length)
            {
                dest = Convert.ToDouble(list[(int)index]);
            }

            return dest;
        }

        private string getAsString(string[] list, DATA_INDEX index)
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