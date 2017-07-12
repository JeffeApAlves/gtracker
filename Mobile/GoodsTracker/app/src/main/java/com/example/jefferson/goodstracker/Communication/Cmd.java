package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Cmd  extends Object implements ObserverCommunication {

    private Header              header;
    private PayLoad             payload;
    private EventReceiveAnswer  eventReceiveAnswer = null;

    public Cmd(int address, int dest,String r,Operation o) {

        header  = new Header(r,o);
        payload = new PayLoad();

        header.setAddress(address);
        header.setDest(dest);

        Communication.getInstance().registerObserver(this);
    }

    public Cmd(int address, int dest,String r,Operation o,EventReceiveAnswer eRA) {

        header  = new Header(r,o);
        payload = new PayLoad();

        header.setAddress(address);
        header.setDest(dest);

        eventReceiveAnswer = eRA;

        Communication.getInstance().registerObserver(this);
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

    public int getDest() {

        return header.getDest();
    }

    /*
     * Metodo chamado para notificar o recebimento de resposta
     */
    @Override
    public void updateAnswer(AnsCmd ans) {

        if(eventReceiveAnswer!=null) {

            eventReceiveAnswer.onReceiveAnswer(ans);
        }
    }
}
