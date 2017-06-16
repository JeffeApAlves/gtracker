using System;

namespace GoodsTracker
{
    internal class DecoderFrameTx : IDecoderFrameTx
    {
        public void buildFrame(out TxFrame frame, Cmd cmd)
        {
            frame = new TxFrame();
            string str = string.Format("[{0}{1}{2}]", cmd.getName(), cmd.Address,cmd.Value);

//            frame.s
        }
    }
}