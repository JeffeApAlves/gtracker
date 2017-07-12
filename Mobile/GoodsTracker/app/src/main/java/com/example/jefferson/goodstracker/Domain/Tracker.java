package com.example.jefferson.goodstracker.Domain;

import com.example.jefferson.goodstracker.Communication.AnsCmd;
import com.example.jefferson.goodstracker.Communication.Cmd;
import com.example.jefferson.goodstracker.Communication.CommunicationUnit;
import com.example.jefferson.goodstracker.Communication.EventReceiveAnswer;
import com.example.jefferson.goodstracker.Communication.Operation;
import com.example.jefferson.goodstracker.Communication.RESOURCE_TYPE;

/**
 * Created by Jefferson on 09/07/2017.
 */

public class Tracker extends CommunicationUnit {

    private DataTelemetria  telemetria;
    private boolean         statusLock;

    public Tracker(int val) {

        super(val);
        statusLock = false;
    }

    public void requestBehavior(EventReceiveAnswer on_ans)
    {
        Cmd cmd = createCMD( RESOURCE_TYPE.TLM,Operation.RD,on_ans);

        sendCMD(cmd);
    }

    public void lockVehicle(EventReceiveAnswer on_ans) {

        Cmd cmd = createCMD( RESOURCE_TYPE.LOCK,Operation.WR,on_ans);

        statusLock = true;
        cmd.append("1");

        sendCMD(cmd);
    }

    public void unLockVehicle(EventReceiveAnswer on_ans) {

        Cmd cmd = createCMD( RESOURCE_TYPE.LOCK,Operation.WR,on_ans);

        statusLock = false;
        cmd.append("0");

        sendCMD(cmd);
    }

    /*
     *
     * Hook para processamento de comandos respondidos
     *
     */
    @Override
    public void onReceiveAnswer(AnsCmd ans) {

        if(ans.getResource().equals(RESOURCE_TYPE.TLM)) {

            updateDataTelemetria(ans);
        }
        else if(ans.getResource().equals(RESOURCE_TYPE.LOCK)){

            telemetria.setStatusLock(statusLock);
        }
    }

    void updateDataTelemetria(AnsCmd ans) {

        telemetria = ans.getTelemetria();
    }

    public DataTelemetria getTelemetria() {

        return telemetria;
    }

    public void setTelemetria(DataTelemetria telemetriaData) {
        this.telemetria = telemetriaData;
    }

    public boolean isStatusLock() {
        return statusLock;
    }

    public void setStatusLock(boolean statusLock) {
        this.statusLock = statusLock;
    }

    public double getValLevel() {

        return telemetria.getValLevel();
    }
}
