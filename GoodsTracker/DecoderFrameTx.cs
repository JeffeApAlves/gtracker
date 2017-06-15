using System;

namespace GoodsTracker
{
    internal class DecoderFrameTx : IDecoderFrameTx
    {
        public void buildFrame(out TxFrame frame, Cmd cmd)
        {
            string str = string.Format("[{0}{1}{2}]", cmd.getName(), cmd.GetAddress(),cmd.GetValue());

//            frame.s
        }
    }
}