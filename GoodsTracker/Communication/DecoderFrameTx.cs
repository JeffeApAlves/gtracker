using System;
using System.Text;

namespace GoodsTracker
{
    // [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[2] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ] \r\n

    internal class DecoderFrameTx : IDecoderFrameTx
    {
        public bool setFrame(out CommunicationFrame frame, CommunicationUnit unit)
        {
            bool ret = false;

            frame = new CommunicationFrame();

            try
            {
                Cmd cmd = unit.getNextCmd();

                StringBuilder sb = new StringBuilder();

                sb.Append(unit.Address);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(cmd.Dest);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(cmd.Operation);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(cmd.Resource);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(0);
                sb.Append(CONST_CHAR.SEPARATOR);
                sb.Append(CONST_CHAR.SEPARATOR);

                frame.Frame = sb.ToString();

                ret = true;
            }
            catch
            {
                ret = false;
            }

            return ret;
        }

        public bool setPayLoad(ref CommunicationFrame frame, Behavior b)
        {
            bool ret = false;

            try
            {
                StringBuilder sb = new StringBuilder();

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

                ret = true;
            }
            catch
            {
                ret = false;
            }

            return ret;
        }
    }
}