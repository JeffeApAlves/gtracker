package com.example.jefferson.goodstracker.Communication;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Jefferson on 08/0`/2017.
 */

abstract public class Communication extends Object implements Communic,ObservableCommunication,Runnable  {

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

    private static WorkerPublish        workerPublish       = null;
    private static WorkerSubscribe      workerSubscribe     = null;

    private static final String CMD_KEY = "cmd";

    protected Communication(){

    }

    /**
     * Metodo construtor do tipo de comunicacao
     *
     * @param type_com
     */
    public static boolean create(TYPE_COMMUNICATION type_com) {

        boolean success = false;

        try {

            if (type != type_com) {

                finish();

                type = type_com;

                if (!TYPE_COMMUNICATION.NONE.equals(type_com)) {

                    switch (type) {

                        case AMQP:      instance    = new AMQPCommunication();  break;
                        case SERIAL:    instance    = null;                     break; // Comunicacao via serial
                    }

                    success = instance!=null;
                }
            }

        }catch (Exception e){

            success = false;
            Log.d("","Problemas na inicializacao da conexao");
        }

        return success;
    }

    /**
     * Inicia a thead que fara a conexao
     *
     */
    protected void startCommunication() {

        new Thread(this).start();
    }

    /**
     *
     * A conexao sera chamada em uma Thread
     *
     */
    @Override
    public void run() {

        try{

            if(connection()){

                createPublishThread();
                createSubscribeThread();

                workerPublish.start();
                workerSubscribe.start();
            }

        }catch (Exception e){

            Log.d("","Problemas na inicialicalizaco da conexao");
        }
    }

    public static void finish(){

        if(workerPublish!=null) {
            workerPublish.interrupt();
        }

        if(workerSubscribe!=null) {
            workerSubscribe.interrupt();
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

    public static Cmd findCmdByAnswer(AnsCmd ans) {

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

        if (isAnyQueueCmd()) {

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

            Cmd cmd = findCmdByAnswer(ans);

            removeTxCmd(cmd);
            removeAns(ans);
        }
    }

    public static Cmd takeFirstCmd() {

        return isAnyQueueCmd()?getArrayOfCmd()[0]:null;
    }

    @Override
    public void registerObserver(ObserverCommunication observer) throws IOException {

        if (observer != null) {

            units.put(observer.getAddress(),observer);
        }
    }

    @Override
    public void removeObserver(ObserverCommunication observer){

        units.remove(observer.getAddress());
    }

    @Override
    public void notifyObserver(Object obj){

        if(obj instanceof AnsCmd) {

            AnsCmd ans = (AnsCmd)obj;

            if (units.containsKey(ans.getHeader().getAddress())) {

                ObserverCommunication ob = units.get(ans.getHeader().getAddress());

                //Notifica apenas entidade respectiva ddo address
                ob.updateAnswer(ans);

                //Notifica o comando que gerou a resposta
                Cmd cmd = findCmdByAnswer(ans);
                if (cmd != null) {

                    cmd.updateAnswer(ans);
                }
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
     * Thread para processamento da pilha de saida
     */
    public void createPublishThread() {

        workerPublish   = new WorkerPublish();
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
    public static void sendPublish(Cmd cmd) {

        addCmd(cmd);
        workerPublish.flush();
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

            //TODO salvar resposta na lista
            //addAns(ans);
            //doSubscribe();
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

            ID_MENSSAGE id = ID_MENSSAGE.values()[msg.what];

            switch (id){

                case CMD:   publishCmd(takeFirstCmd());
            }

            return true;
        }

        public void flush() {

            try {
                Message messageToSend = handler.obtainMessage();

                Bundle bundle = new Bundle();

                bundle.putString(CMD_KEY, "");
                messageToSend.what = ID_MENSSAGE.CMD.ordinal();
                messageToSend.setData(bundle);

                handler.sendMessage(messageToSend);

            }catch (Exception e){

                System.out.print(e.toString());
                Log.d("",e.getClass().toString());
            }
        }
    }
}