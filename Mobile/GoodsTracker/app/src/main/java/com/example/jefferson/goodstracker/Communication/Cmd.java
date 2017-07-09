package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Cmd  implements ObserverAnswerCmd {

    Header              header;
    PayLoad             payload;
    EventReceiveAnswer  eventReceiveAnswer = null;

    public Cmd(int address, int dest,String r,Operation o,EventReceiveAnswer eRA) {

        header  = new Header(r,o);
        payload = new PayLoad();

        header.setAddress(address);
        header.setDest(dest);

        eventReceiveAnswer = eRA;

        Communication.getCommunic().registerObserver(this);
    }

    public void append(String str) {
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

    public void setAddress(int address){

        header.setAddress(address);
    }

    public void setDest(int dest){

        header.setDest(dest);
    }

    @Override
    public int getAddress() {

        return header.getAddress();
    }

    /*
     * Metodo chamado para notificar o recebimento de resposta
     */
    @Override
    public void updateAnswer(AnsCmd ans) {

        eventReceiveAnswer.onReceiveAnswer();
    }
}
