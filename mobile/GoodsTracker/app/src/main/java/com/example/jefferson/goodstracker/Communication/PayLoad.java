package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class PayLoad  extends Object{

    public final int        LEN_MAX_PAYLOAD = 256;
    private StringBuilder   data;

    public PayLoad(String val){

        data = new StringBuilder(val);
    }

    public PayLoad(){

        clear();
    }

    public void append(char b) {

        data.append(b);
    }

    public void append(String b) {

        data.append(b);
    }

    public void append(boolean b) {

        data.append(String.valueOf((b?1:0)));
    }

    public void append(double b) {

        //TODO verificar o que o parametro G faz na conversao string
        //data += b.toString("G");

        data.append(b);
    }

    public int length() {

        return data==null? 0:data.length();
    }

    public boolean isFull() {

        return length() >= LEN_MAX_PAYLOAD;
    }

    public boolean isEmpty() {

        return length() <= 0;
    }

    public void clear(){
        data = new StringBuilder();
    }

    public String getData() {

        return data!=null?data.toString():"";
    }

    public void setData(String data) {

        this.data = new StringBuilder(data);
    }
}
