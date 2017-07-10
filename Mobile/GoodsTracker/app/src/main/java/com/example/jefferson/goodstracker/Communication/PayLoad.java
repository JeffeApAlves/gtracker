package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class PayLoad {

    public final int    LEN_MAX_PAYLOAD = 256;
    private String      data            = "";

    public PayLoad(){

        clear();
    }

    public void append(char b)
    {
        data += b;
    }

    public void append(String b)
    {
        data += b;
    }

    public void append(boolean b)
    {
        data += String.valueOf((b?1:0));
    }


    public void append(double b) {

        //TODO verificar o que o parametro G faz na conversao string
        //data += b.toString("G");

        data += Double.toString(b);
    }

    public int length()
    {
        return data==null? 0:data.length();
    }

    public boolean isFull()
    {
        return length() >= LEN_MAX_PAYLOAD;
    }

    public boolean isEmpty()
    {
        return length() <= 0;
    }

    public String str() {

        return String.format("%03d",length()) + CONST_COM.CHAR.SEPARATOR + data + CONST_COM.CHAR.SEPARATOR;
    }

    public void clear()
    {
        data = "";
    }

    public char[] toCharArray() {

        return str().toCharArray();
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }
}
