package com.example.jefferson.goodstracker.Communication;

import android.util.Log;

/**
 * Created by Jefferson on 08/07/2017.
 */

abstract public class CommunicationUnit implements ObserverAnswerCmd {

    /*
     * Endereco da unidade
     */
    protected int   address;

    public CommunicationUnit(int val) {

        address = val;

        Communication.getCommunic().registerObserver(this);
    }

    public Cmd createCMD(int dest, Operation o, String resource,EventReceiveAnswer on_ans) {

        Cmd cmd  = new Cmd( CONST_COM.MASTER.ADDRESS,
                            dest,
                            resource,
                            o,on_ans);

        cmd.setAddress(CONST_COM.MASTER.ADDRESS);
        cmd.setDest(dest);

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

            // Pega o comando respectivo
            Cmd cmd = Communication.searchCmdOfAnswer(ans);

            if (cmd != null) {

                // Executa evento de recebmento de resposta de comando
                onReceiveAnswer(ans);
            }
        }
        catch (Exception e) {

            System.out.println("Erro na execucao das callbacks de communicacao");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }
    }

    abstract public void onReceiveAnswer(AnsCmd ans);
}
