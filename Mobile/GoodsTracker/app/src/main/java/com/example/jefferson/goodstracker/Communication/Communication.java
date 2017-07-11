package com.example.jefferson.goodstracker.Communication;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;
import com.rabbitmq.client.AMQP;
import com.rabbitmq.client.QueueingConsumer;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeoutException;


/**
 * Created by Jefferson on 08/07/2017.
 */

abstract public class Communication extends Object implements Communic,ObservableCommunication  {

    // ID para iidentificacao das mensagens
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

    private Thread                      connThread,
                                        subscribeThread,
                                        publishThread;

    private Handler publishHandle,subscribeHandle;

    private static final String CMD_KEY = "cmd";

    //Incia a htrea que fara a conexao
    @Override
    public void init() {

        createPublishThread();
        createSubscribeThread();
        createConnThread();
    }

    @Override
    public void deInit(){
        connThread.interrupt();
        subscribeThread.interrupt();
        publishThread.interrupt();
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
        connThread.start();
    }

    /**
     *
     * Thread para processamento da pilha de saida
     */
    public void createPublishThread() {

        publishThread = new Thread(new TaskPublish());
        publishThread.start();
    }

    /**
     * Thread para processamento da pilha de entrada
     *
     */
    public void createSubscribeThread() {

        subscribeThread = new Thread(new TaskSubscribe());
        subscribeThread.start();
    }

    /**
     * Envia um comando para a fila de mensagens da Thread de transmissao
     *
     * @param cmd
     */
    private void sendMessageToPublish(Cmd cmd) {

        DataFrame frame = new DataFrame(cmd);

        Message messageToSend = publishHandle.obtainMessage(ID_MENSSAGE.CMD.ordinal());

        Bundle bundle = new Bundle();
        bundle.putString(CMD_KEY, frame.str());
        messageToSend.setData(bundle);

        publishHandle.sendMessage(messageToSend);
    }

    class TaskStartConnection implements  Runnable{

        @Override
        public void run() {

            synchronized (connThread){

                if(connection()){

                    // Delay para garantir que as Threads de transmissao e recepcao ja foram
                    // inicializadas e estao em execucao esperando a notificacao
                    try {
                        connThread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    connThread.notifyAll();
                }
            }
        }
    }

    class TaskPublish implements Runnable{

        @Override
        public void run() {

            // Espera a conexao
            synchronized (connThread){

                try {

                    connThread.wait();

                } catch (InterruptedException e) {

                    e.printStackTrace();
                }
            }

            // Loop infinito para processamento do subscribe
            while(true) {

                doPublish();

                try {
                    Thread.sleep(time);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    class TaskSubscribe implements Runnable{

        @Override
        public void run() {

            // Espera a conexao
            synchronized (connThread){

                try {

                    connThread.wait();

                } catch (InterruptedException e) {

                    e.printStackTrace();
                }
            }

            // Loop infinito para processamento do subscribe
            while(true) {

                Looper.prepare();

                publishHandle = new publishHandler();

                Looper.loop();


                try {
                    Thread.sleep(time);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     *
     * Hook para processamento das mensagens
     *
     */
    private class publishHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {

            ID_MENSSAGE id = ID_MENSSAGE.values()[msg.what];

            String teste = msg.getData().getString(CMD_KEY);

            switch (id){

                case CMD: doPublish();    break;
            }
        }
    }
}