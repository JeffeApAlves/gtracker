package com.example.jefferson.goodstracker.Communication;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * Created by Jefferson on 08/07/2017.
 */

abstract public class Communication extends Object implements Communic,ObservableCommunication  {

    protected int  time                                     = CONST_COM.CONFIG.TIME_COMMUNICATION;
    private static Communication        instance            = null;
    private static Thread               thread              = null;
    private static TYPE_COMMUNICATION   type                = TYPE_COMMUNICATION.NONE;
    private static Map<String,Cmd>      txCmds              = new HashMap<String, Cmd>();
    private static Map<String,Cmd>      cmds                = new HashMap<String, Cmd>();
    private static List<AnsCmd>         answers             = new ArrayList<AnsCmd>();
    private static Map<Integer,ObserverCommunication>units     = new HashMap<Integer,ObserverCommunication>();

    @Override
    public void init() {

        initThread();
    }

    @Override
    public void deInit(){

        thread.interrupt();
    }

    public static void create(TYPE_COMMUNICATION t) {

        if (type != t) {

            type = t;

            if(instance!=null){

                instance.deInit();
            }

            switch (type) {

                case AMQP: instance = new AMQPCommunication(); break;
            }

            instance.init();
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

            if ((   ans_header.getResource()    == cmd_header.getResource()) &&
                (   ans_header.getDest()        == cmd.getAddress()) &&
                (   ans_header.getCount()       == cmd_header.getCount())    &&
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

    public static Cmd getNextCmd() {

        return isAnyQueueCmd()?getArrayOfCmd()[0]:null;
    }

    /**
     *
     * Inicializa a thread que ira gerenciar as pilhas de mensagens
     */
    void initThread(){

        thread = new Thread(new Runnable() {

            @Override
            public void run() {

                doCommunication();

                try {
                    Thread.sleep(time);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
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

            //Notifica a entidade respectiva da unidade
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
}