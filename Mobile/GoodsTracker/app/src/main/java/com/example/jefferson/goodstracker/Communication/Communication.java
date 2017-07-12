package com.example.jefferson.goodstracker.Communication;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Executors;


/**
 * Created by Jefferson on 08/0`/2017.
 */

abstract public class Communication extends Object implements Communic,ObservableCommunication  {

    // ID para identificacao das mensagens
    enum ID_MENSSAGE{

        CMD,
    };

    protected int  time                                     = CONST_COM.CONFIG.TIME_COMMUNICATION;
    private static Communication        instance            = null;
    private static TYPE_COMMUNICATION   type                = TYPE_COMMUNICATION.NONE;
    private static Map<String,Cmd>      txCmds              = new HashMap<String, Cmd>();
    private static Map<String,Cmd>      cmds                = new HashMap<String, Cmd>();
    private static List<AnsCmd>         answers             = new ArrayList<AnsCmd>();
    private static Map<Integer,ObserverCommunication>units  = new HashMap<Integer,ObserverCommunication>();

    private Thread              connThread;
    private WorkerPublish       workerPublish;
    private WorkerSubscribe     workerSubscribe;

    private static final String CMD_KEY = "cmd";

    //Incia a htrea que fara a conexao
    @Override
    public void init() {

        createPublishThread();
        createSubscribeThread();
        createConnThread();

        connThread.start();
    }

    @Override
    public void deInit(){

        connThread.interrupt();
    }

    public static void create(TYPE_COMMUNICATION t) {

        if (type != t) {

            if(instance!=null){

                instance.deInit();
            }

            type = t;

            if(!TYPE_COMMUNICATION.NONE.equals(t)) {

                switch (type) {

                    case AMQP:
                        instance = new AMQPCommunication();
                        break;
                    case SERIAL:
                        instance = null;
                        break; // Comunicacao via serial
                }

                instance.init();
            }
        }
    }

    public static boolean isAnyTxCmd() {

        return txCmds.size() > 0;
    }


    public static boolean isAnyAns() {

        return answers.size() > 0;
    }

    public static boolean isAnyQueueCmd() {

        return cmds.size() > 0;
    }

    public static void addAns(AnsCmd ans) {

        if (ans != null) {

            answers.add(ans);
        }
    }

    public static void removeAns(AnsCmd ans) {

        if (ans != null) {

            answers.remove(ans);
        }
    }

    public static void removeTxCmd(Cmd cmd) {

        if (cmd != null) {

            txCmds.remove(cmd.getResource());
        }
    }

    public static void addTxCmd(Cmd cmd) {

        if (cmd != null) {

            txCmds.put(cmd.getResource(),cmd);
        }
    }

    public static void removeCmd(Cmd cmd) {

        if (cmd != null) {

            cmds.remove(cmd.getResource());
        }
    }

    public static void addCmd(Cmd cmd) {

        if (cmd != null) {

            cmds.put(cmd.getResource(),cmd);
        }
    }

    public static Cmd searchCmdOfAnswer(AnsCmd ans) {

        Cmd cmd = null;

        Header ans_header = ans.getHeader();

        if(txCmds.containsKey(ans_header.getResource())) {

            Header cmd_header = txCmds.get(ans_header.getResource()).getHeader();

            if ((   ans_header.getResource()    == cmd_header.getResource())    &&
                (   ans_header.getDest()        == cmd.getAddress())            &&
                (   ans_header.getCount()       == cmd_header.getCount())       &&
                    ans_header.getAddress()     == cmd.getDest()) {

                return cmd;
            }
        }

        return null;
    }

    public static CommunicationUnit[] getArrayOfUnit() {

        CommunicationUnit[] ret = null;

        if (units.size() > 0) {

            //TODO Verificar a conversao de map para array
            ret = units.values().toArray(new CommunicationUnit[units.size()]);
        }

        return ret;
    }

    public static Cmd[] getArrayOfCmd() {

        Cmd[] ret = null;

        if (cmds.size() > 0) {

            //TODO Verificar a conversao de map para array
            ret = cmds.values().toArray(new Cmd[cmds.size()]);
        }

        return ret;
    }

    @Override
    public void acceptAnswer(AnsCmd ans) {

        if (ans != null) {

            addAns(ans);

            notifyObserver(ans);

            Cmd cmd = searchCmdOfAnswer(ans);

            removeTxCmd(cmd);
            removeAns(ans);
        }
    }

    public static Cmd takeFirst() {

        return isAnyQueueCmd()?getArrayOfCmd()[0]:null;
    }

    @Override
    public void registerObserver(ObserverCommunication observer){

        if (observer != null) {

            units.put(observer.getAddress(),observer);
        }
    }

    @Override
    public void removeObserver(ObserverCommunication observer){

        units.remove(observer.getAddress());
    }

    @Override
    public void notifyAllObservers(AnsCmd ans){

        //Notifica todas as unidades
        for (ObserverCommunication ob : units.values()){

            ob.updateAnswer(ans);
        }

        //Notifica o comando que gerou a resposta
        Cmd cmd = searchCmdOfAnswer(ans);
        if(cmd!=null) {

            cmd.updateAnswer(ans);
        }
    }

    @Override
    public void notifyObserver(AnsCmd ans){

        if(units.containsKey(ans.getHeader().getAddress())) {

            ObserverCommunication ob = units.get(ans.getHeader().getAddress());

            //Notifica apenas entidade respectiva ddo address
            ob.updateAnswer(ans);

            //Notifica o comando que gerou a resposta
            Cmd cmd = searchCmdOfAnswer(ans);
            if (cmd != null) {

                cmd.updateAnswer(ans);
            }
        }
    }

    /**
     *
     * Retorna o singleton
     *
     * @return
     */
    public static Communication getInstance() {
        return instance;
    }

    public void setTime(int time) {
        this.time = time;
    }

    /**
     *
     * Thread para inicializacao da conexao
     */
    public void createConnThread(){

        connThread = new Thread(new TaskStartConnection());
    }

    /**
     *
     * Thread para processamento da pilha de saida
     */
    public void createPublishThread() {

        workerPublish = new WorkerPublish();
    }

    /**
     * Thread para processamento da pilha de entrada
     *
     */
    public void createSubscribeThread() {

        workerSubscribe = new WorkerSubscribe();
    }

    /**
     * Envia um comando para a fila de mensagens da Thread de transmissao
     *
     * @param cmd
     */
    public void sendPublish(Cmd cmd) {

        addCmd(cmd);
        workerPublish.sendMessageToPublish(cmd);

    }

    private class TaskStartConnection implements  Runnable{

        @Override
        public void run() {

            if(connection()){

                workerPublish.start();
                workerSubscribe.start();
            }
        }
    }

    /**
     *
     * Hook para processamento das mensagens
     *
     */
    private class WorkerSubscribe extends Thread implements Handler.Callback {

        public Handler handler;

        @Override
        public void run() {

            Looper.prepare();
            handler = new Handler(this);
            Looper.loop();
        }

        @Override
        public boolean  handleMessage(Message msg) {

            doSubscribe();
            return true;
        }
    }

    private class WorkerPublish extends Thread implements Handler.Callback {

        public Handler handler;

        @Override
        public void run() {

            Looper.prepare();
            handler = new Handler(this);

            Looper.loop();
        }

        @Override
        public boolean  handleMessage(Message msg) {

            doPublish();
            return true;
        }

        public void sendMessageToPublish(Cmd cmd) {

            try {
                DataFrame frame = new DataFrame(cmd);

                Message messageToSend = handler.obtainMessage();

                Bundle bundle = new Bundle();
                bundle.putString(CMD_KEY, frame.str());
                messageToSend.what = ID_MENSSAGE.CMD.ordinal();
                messageToSend.setData(bundle);

                handler.sendMessage(messageToSend);
            }catch (Exception e){

                System.out.print(e.toString());

                Log.d("",e.toString());
            }
        }
    }
}