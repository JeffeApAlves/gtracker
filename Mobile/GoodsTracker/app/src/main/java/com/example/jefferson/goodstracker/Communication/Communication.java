package com.example.jefferson.goodstracker.Communication;

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

    private static Communication        instance            = null;
    private static TYPE_COMMUNICATION   type                = TYPE_COMMUNICATION.NONE;
    private static Map<String,Cmd>      txCmds              = new HashMap<String, Cmd>();
    private static Map<String,Cmd>      cmds                = new HashMap<String, Cmd>();
    private static List<AnsCmd>         answers             = new ArrayList<AnsCmd>();
    private static Map<Integer,ObserverCommunication>units  = new HashMap<Integer,ObserverCommunication>();

    private static WorkerPublish        workerPublish       = null;
    private static WorkerSubscribe      workerSubscribe     = null;

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

                deInit();

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

            if(connect()){

                startWorkerPublish();
                startWorkerSubscribe();
            }

        }catch (Exception e){

            Log.d("","Problemas na inicialicalizaco da conexao");
        }
    }

    public static void deInit(){

        if(workerPublish!=null) {
            workerPublish.interrupt();
        }

        if(workerSubscribe!=null) {
            workerSubscribe.interrupt();
        }

        if(instance!=null) {
            instance.close();
        }
    }

    public static boolean isAnyTxCmd() {

        return txCmds.size() > 0;
    }


    public static boolean isAnyAns() {

        return answers.size() > 0;
    }

    public static boolean isAnyCmd() {

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

        if (isAnyCmd()) {

            //TODO Verificar a conversao de map para array
            ret = cmds.values().toArray(new Cmd[cmds.size()]);
        }

        return ret;
    }

    public static AnsCmd[] getArrayOfAns() {

        AnsCmd[] ret = null;

        if (isAnyAns()) {

            ret = new AnsCmd[answers.size()];
            answers.toArray(ret);
        }

        return ret;
    }

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

        return isAnyCmd()?getArrayOfCmd()[0]:null;
    }

    public static AnsCmd takeFirstAns() {

        return isAnyAns()?getArrayOfAns()[0]:null;
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

                // Notifica apenas entidade respectiva do address
                ob.updateAnswer(ans);
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



    /**
     *
     * Thread para processamento da pilha de saida
     */
    @Override
    public void startWorkerPublish() {

        workerPublish   = new WorkerPublish();
        workerPublish.start();
    }

    /**
     * Thread para processamento da pilha de entrada
     *
     */
    @Override
    public void startWorkerSubscribe() {

        workerSubscribe = new WorkerSubscribe();
        workerSubscribe.start();
    }

    /**
     * Envia um comando para a fila de mensagens da Thread de transmissao
     *
     * @param cmd
     */
    public void publish(Cmd cmd) {

        addCmd(cmd);
        workerPublish.flush(IdMessage.CMD);
    }

    /**
     *
     * @param ans
     */
    public void publish(AnsCmd ans) {

        // TODO Criar lista de resposta para ser usada como slave
        workerPublish.flush(IdMessage.ANS);
    }

    /**
     * Processa o frame recebido
     *
     * @param frame
     */
    @Override
    public void subscribe(DataFrame frame){

        AnsCmd      ans = new AnsCmd();

        if(Decoder.frame2Ans(frame,ans)){

            acceptAnswer(ans);
        }
    }

    public Handler getHandlerSubscribe(){

        return workerSubscribe.getHandler();
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

            IdMessage id  = IdMessage.values()[msg.what];

            String data  = msg.getData().getString("PAYLOAD");

            DataFrame frame = new DataFrame();
            frame.setData(data);

            subscribe(frame);

            return true;
        }

        public Handler getHandler() {
            return handler;
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

            doWork(IdMessage.values()[msg.what]);

            return true;
        }

        public void doWork(IdMessage i) {

            boolean flg = false;

            DataFrame frame = new DataFrame();

            switch (i){

                case ANS:   flg = Decoder.ans2Frame(takeFirstAns(),frame);break;
                case CMD:   flg = Decoder.cmd2Frame(takeFirstCmd(),frame);break;
            }

            if(flg){

                publish(frame);
            }
        }

        public Handler getHandler() {
            return handler;
        }

        /**
         * O conteudo foi previamente inserido nos containers respectivo
         */
        public void flush(IdMessage id) {

            handler.sendEmptyMessage(id.ordinal());
        }
    }
}