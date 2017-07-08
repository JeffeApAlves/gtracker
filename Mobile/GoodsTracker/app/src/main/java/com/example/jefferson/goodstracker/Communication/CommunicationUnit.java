package com.example.jefferson.goodstracker.Communication;

import android.util.Log;

/**
 * Created by Jefferson on 08/07/2017.
 */

abstract public class CommunicationUnit {

    //TODO implementar o delegate de resposta
    //internal delegate ResultExec onAnswerCmd(AnsCmd ans);

    /*
     * Endereco da unidade
     */
    protected int   address;
    protected abstract void onReceiveAnswer(AnsCmd ans);

    public CommunicationUnit(int val)
    {
        address = val;
        Communication.AddUnit(this);
    }

    public Cmd createCMD(int dest, Operation o, String resource)
    {
        Cmd cmd               = new Cmd(resource, o);

        cmd.getHeader().setAddress(CONST_COM.MASTER.ADDRESS);
        cmd.getHeader().setDest(dest);

        return cmd;
    }

    public void sendCMD(Cmd cmd) {

        Communication.addCmd(cmd);
    }

    public void processAnswer(AnsCmd ans) {

        try {

            // Pega o comando respectivo
            Cmd cmd = Communication.searchCmdOfAnswer(ans);

            if (cmd != null) {

                // Executa evento de recebmento de resposta de comando
                onReceiveAnswer(ans);

                // TODO chamada da call do comando
                // Executa call back respectiva do comando
                //cmd.EventAnswerCmd?.Invoke(ans);
            }
        }
        catch (Exception e) {

            System.out.println("Erro na execucao das callbacks de communicacao");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }
    }

    public int getAddress() {
        return address;
    }

    public void setAddress(int address) {
        this.address = address;
    }
}
