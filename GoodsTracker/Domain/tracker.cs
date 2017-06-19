using System;

namespace GoodsTracker
{
    class Tracker : CommunicationUnit,InterfaceTracker
    {
        Behavior    behavior;
        bool        lockStatus = false;

        internal Tracker()
        {
            Address = 1;
        }

        public Behavior getBehavior()
        {
            throw new NotImplementedException();
        }

        public void getLevel()
        {
            throw new NotImplementedException();
        }

        public void requestBehavior(CallBackAnsCmd ans)
        {
            sendCMD(2,Operation.RD,RESOURCE.BEHAVIOR).setCallBack(callBackData1);
        }

        public void lockVehicle(CallBackAnsCmd ans)
        {
            sendCMD(2, Operation.WR, RESOURCE.LOCK).setCallBack(ans);
        }

        public void unLockVehicle(CallBackAnsCmd ans)
        {
            sendCMD(2, Operation.WR, RESOURCE.LOCK).setCallBack(ans);
        }

        public void unLockVehicle()
        {
            sendCMD(2, Operation.WR, RESOURCE.LOCK).setCallBack(callBackUnLock);
        }

        public void LockVehicle()
        {
            sendCMD(2, Operation.WR, RESOURCE.LOCK).setCallBack(callBackLock);
        }

        private ResultExec callBackUnLock(AnsCmd ans)
        {
            lockStatus = false;
            return ResultExec.EXEC_SUCCESS;
        }

        private ResultExec callBackLock(AnsCmd ans)
        {
            lockStatus = true;
            return ResultExec.EXEC_SUCCESS;
        }

        private ResultExec callBackData1(AnsCmd ans)
        {
            behavior = new Behavior();

            behavior.AxisX.Acceleration.Val = ans.DadosRx.X.acceleration;

            return ResultExec.EXEC_SUCCESS;
        }
    }
}
