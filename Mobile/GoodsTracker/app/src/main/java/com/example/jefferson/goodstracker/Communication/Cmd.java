package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Cmd {

// TODO implementar se necessario o ponteiro de callback do comando
//    onAnswerCmd onAnswerCmd;
    Header      header;
    PayLoad     payload;

    public Cmd(String r,Operation o)
    {
        header  = new Header();
        payload = new PayLoad();

        header.setResource(r);
        header.setOperation(o);
    }

    public void append(String str)
    {
        payload.append(str);
    }
}
