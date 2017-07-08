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

    public Header getHeader() {
        return header;
    }

    public void setHeader(Header header) {
        this.header = header;
    }

    public PayLoad getPayload() {
        return payload;
    }

    public void setPayload(PayLoad payload) {
        this.payload = payload;
    }

    public  String getResource(){

        return header.getResource();
    }
}
