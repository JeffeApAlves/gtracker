package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 17/07/2017.
 */

public class ChatMessage {

    Header  header  = new Header();
    PayLoad payload = new PayLoad();


    public void setMessage(String msg){

        payload.setData(msg);
    }

    public Header getHeader() {
        return header;
    }

    public void setHeader(Header header) {
        this.header = header;
    }

    public PayLoad getPayLoad() {
        return payload;
    }

    public void setPayLoad(PayLoad payLoad) {
        this.payload = payLoad;
    }

    public  int getAddress(){

        return 0;
    }
}
