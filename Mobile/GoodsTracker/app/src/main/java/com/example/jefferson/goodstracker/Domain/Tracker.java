package com.example.jefferson.goodstracker.Domain;

import com.example.jefferson.goodstracker.Communication.AnsCmd;
import com.example.jefferson.goodstracker.Communication.Cmd;
import com.example.jefferson.goodstracker.Communication.CommunicationUnit;
import com.example.jefferson.goodstracker.Communication.Operation;
import com.example.jefferson.goodstracker.Communication.RESOURCE_TYPE;

import java.io.IOException;

/**
 * Created by Jefferson on 09/07/2017.
 */

public class Tracker extends CommunicationUnit {

    private DataTelemetria  tlm = new DataTelemetria();

    public Tracker(int val) throws IOException {

        super(val);
    }

    public void requestBehavior() {

        Cmd cmd = createCMD( RESOURCE_TYPE.TLM,Operation.RD);

        sendCMD(cmd);
    }

    public void lockVehicle() {

        Cmd cmd = createCMD( RESOURCE_TYPE.LOCK,Operation.WR);

        cmd.append("1");

        sendCMD(cmd);
    }

    public void unLockVehicle() {

        Cmd cmd = createCMD( RESOURCE_TYPE.LOCK, Operation.WR);

        cmd.append("0");

        sendCMD(cmd);
    }

    /*
     *
     * Hook para processamento das respostas dos comandos
     *
     */
    @Override
    public void onReceiveAnswer(AnsCmd ans) {

        if(ans.getResource().equals(RESOURCE_TYPE.TLM)) {

            // Atualiza a entidade de telemetria
            updateDataTelemetria(ans);
        }
        else if(ans.getResource().equals(RESOURCE_TYPE.LOCK)){

            // Nothing to do at the moment
        }
    }

    void updateDataTelemetria(AnsCmd ans) {

        tlm = ans.getTelemetria();
    }

    public DataTelemetria getTlm() {

        return tlm;
    }

    public void setTlm(DataTelemetria telemetriaData) {
        this.tlm = telemetriaData;
    }

    public LockStatus getStatusLock() {
        return tlm.getStatusLock();
    }

    public void setStatusLock(LockStatus statusLock) {

        if(statusLock!=getStatusLock()) {

            if (statusLock.equals(LockStatus.LOCK)) {

                lockVehicle();

            } else {

                unLockVehicle();
            }
        }
    }

    public double getValLevel() {

        return tlm.getValLevel();
    }

    public Notification[] getNotifications() {

        Notification[] teste  = {   new Notification("A", Notification.type_notification.DANGER),
                                    new Notification("B", Notification.type_notification.OK),
                                    new Notification("C", Notification.type_notification.WARNING),
                                    new Notification("D", Notification.type_notification.DANGER)
                                };

        return teste;
    }
}
