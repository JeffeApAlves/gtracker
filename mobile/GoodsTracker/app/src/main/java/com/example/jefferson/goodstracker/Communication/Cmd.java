package com.example.jefferson.goodstracker.Communication;

import java.io.IOException;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Cmd  extends Object{

    private Header      header;
    private PayLoad     payload;

    public Cmd(){

        header  = new Header();
        payload = new PayLoad();
    }

    public Cmd(int address, int dest,String r,Operation o){

        header  = new Header(r,o);
        payload = new PayLoad();

        header.setAddress(address);
        header.setDest(dest);
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

    public int getAddress() {

        return header.getAddress();
    }

    public int getDest() {

        return header.getDest();
    }
}
