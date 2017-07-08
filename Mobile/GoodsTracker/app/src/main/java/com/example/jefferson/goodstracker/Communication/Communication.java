package com.example.jefferson.goodstracker.Communication;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * Created by Jefferson on 08/07/2017.
 */

public class Communication extends Object {

    private static Thread               threadCommunication;
    private static TYPE_COMMUNICATION   type;
    private static Communic             communic        = null;
    private static Map<String,Cmd>      txCmds          = new HashMap<String, Cmd>();
    private static Map<String,Cmd>      cmds            = new HashMap<String, Cmd>();
    private static List<AnsCmd>         queueAnsCmd     = new ArrayList<AnsCmd>();
    private static Map<Integer,CommunicationUnit> units = new HashMap<Integer,CommunicationUnit>();

    public static void AddUnit(CommunicationUnit unit) {

        if (unit != null) {

            units.put(unit.getAddress(),unit);
        }
    }

    public static void Init() {

        if (communic != null) {

            communic.Init();
        }

        initThread();
    }

    public static void deInit() {

        if (communic != null) {

            communic.DeInit();
        }
    }

    public static void create(TYPE_COMMUNICATION t) {

        if (type != t) {

            type = t;
            deInit();

            switch (type) {

                case AMQP: communic = new AMQPCommunication(); break;
            }

            Init();
        }
    }

    public static boolean isAnyTxCmd() {

        return txCmds.size() > 0;
    }


    public static boolean isAnyAns() {

        return queueAnsCmd.size() > 0;
    }

    public static boolean isAnyQueueCmd() {

        return cmds.size() > 0;
    }

    public static void addAns(AnsCmd ans) {

        if (ans != null) {

            queueAnsCmd.add(ans);
        }
    }

    public static void removeAns(AnsCmd ans) {

        if (ans != null) {

            queueAnsCmd.remove(ans);
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

            if ((ans_header.getResource() != cmd_header.getResource()) ||
                (ans_header.getDest() != cmd_header.getAddress()) ||
                (ans_header.getCount()!= cmd_header.getCount())) {

                cmd = null;
            }
        }

        return cmd;
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

    public static void publishAnswer(DataFrame frame) {

        communic.publishAnswer(frame);
    }

    public static void acceptAnswer(AnsCmd ans) {

        if (ans != null) {

            Cmd cmd = searchCmdOfAnswer(ans);

            addAns(ans);

            units.get((ans.header.getAddress())).processAnswer(ans);

            removeTxCmd(cmd);
            removeAns(ans);
        }
    }

    public static Cmd getNextCmd() {

        return isAnyQueueCmd()?getArrayOfCmd()[0]:null;
    }

    /*
    Iniciaaliza a thread que ira gerenciar as pilhas de mensagens
     */
    static void initThread(){

        threadCommunication = new Thread(new Runnable() {

            @Override
            public void run() {

                communic.doCommunication();

                try {
                    Thread.sleep(CONST_COM.CONFIG.TIME_COMMUNICATION);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
    }
}
