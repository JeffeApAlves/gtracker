using System;
using System.Text;

namespace GoodsTracker
{
    // [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[2] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ] \r\n

    internal class DecoderFrameTx : IDecoderFrameTx
    {
        public void setFrame(out TxFrame frame, CommunicationUnit unit)
        {
            frame               = new TxFrame();
            StringBuilder sb    = new StringBuilder();

            sb.Append(unit.Address);
            sb.Append(CONST_CHAR.SEPARATOR);
            sb.Append(unit.getNextCmd().Dest);
            sb.Append(CONST_CHAR.SEPARATOR);
            sb.Append(unit.getNextCmd().Operation);
            sb.Append(CONST_CHAR.SEPARATOR);
        }
    }
}