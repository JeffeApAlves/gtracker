package com.example.jefferson.goodstracker.Communication;

import com.example.jefferson.goodstracker.Domain.ThreadRun;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Communication extends ThreadRun {

    private static TYPE_COMMUNICATION type;
    private static Communic communic = null;
    private static Map<Integer,CommunicationUnit> units = new HashMap<Integer,CommunicationUnit>();
    private static Map<String,  Cmd> txCmds             = new HashMap<String, Cmd>();
    private static Map<String,  Cmd> cmds               = new HashMap<String, Cmd>();
    private static List<AnsCmd> queueAnsCmd             = new ArrayList<AnsCmd>();

    public Communication() {

        setTime(CONST_COM.CONFIG.TIME_COMMUNICATION);
    }

    public static void AddUnit(CommunicationUnit unit) {

        if (unit != null) {

            units.put(unit.getAddress(),unit);
        }
    }

    @Override
    public void run() {

        if (communic != null) {

            communic.doCommunication();
        }
    }

    public static void Init() {

        if (communic != null) {

            communic.Init();
        }
    }

    public static void DeInit() {

        if (communic != null) {

            communic.DeInit();
        }
    }

    public static void create(TYPE_COMMUNICATION t) {

        if (type != t || communic == null)
        {
            type = t;
            DeInit();

            switch (type)
            {
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

        if (ans != null)
        {
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


    public static void publishAnswer(DataFrame frame)
    {
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

        Cmd cmd = null;

        if (isAnyQueueCmd()) {

            Cmd temp[] = getArrayOfCmd();

            cmd = temp[0];
        }

        return cmd;
    }
}
