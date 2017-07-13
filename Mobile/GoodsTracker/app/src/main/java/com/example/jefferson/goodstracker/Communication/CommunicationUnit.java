package com.example.jefferson.goodstracker.Communication;

import android.util.Log;

import java.io.IOException;

/**
 * Created by Jefferson on 08/07/2017.
 */

abstract public class CommunicationUnit  extends Object implements ObserverCommunication {

    /*
     * Endereco da unidade
     */
    protected int   address;

    public CommunicationUnit(int val) throws IOException {

        address = val;

        Communication.getInstance().registerObserver(this);
    }

    public Cmd createCMD(String resource,Operation o){

        Cmd cmd  = new Cmd( CONST_COM.MASTER.ADDRESS,
                            address,
                            resource,
                            o);

        return cmd;
    }

    public void sendCMD(Cmd cmd) {

        Communication.addCmd(cmd);
    }

    public int getAddress() {
        return address;
    }

    public void setAddress(int address) {
        this.address = address;
    }

    /*
     * Notifica a unidade
     */
    @Override
    public void updateAnswer(AnsCmd ans){

        try {

            // Executa evento de recebmento de resposta de comando
            onReceiveAnswer(ans);
        }
        catch (Exception e) {

            System.out.println("Erro na execucao da callback pela unidade");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }
    }

    abstract public void onReceiveAnswer(AnsCmd ans);
}
