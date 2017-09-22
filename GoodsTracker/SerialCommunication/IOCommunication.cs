namespace GoodsTracker
{
    interface IOCommunication
    {
        bool Open();
        bool Close();
        void putTxData(char[] v);
        void putRxData(char[] v);
        bool getRxData(out char ch);
        bool hasAnyData();
    }
}
