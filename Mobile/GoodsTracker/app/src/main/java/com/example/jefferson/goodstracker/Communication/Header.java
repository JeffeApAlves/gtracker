package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Header {

    public final int SIZE = 27;             // 5+5+5+2+3+3 + 4 separadores

    String      data;
    int         dest;
    int         address;
    int         count = 0;
    Operation   operation;
    String      resource;
    int         sizePayLoad;

    public Header() {

        count       = 0;
        address     = 0;
        dest        = 0;
        resource    = "";
        operation   = Operation.NN;
        sizePayLoad = 0;
    }

    public String str() {

        data = "";

        DecoderFrame.setHeader(this);

        return data;
    }

    public void Clear() {

        data = "";
    }

    public void append(char b) {

        data += b;
    }

    public void append(String b) {

        data += b;
    }

    public void append(double b) {

        //TODO verificar o que o parametro G faz na conversao string
        //data += b.toString("G");

        data += Double.toString(b);
    }

    public void setData(DataFrame frame) {

        String value = frame.getData();

        if (value.length() > SIZE) {

            data = value.substring(0, SIZE);

        } else {

            data = value;
        }
    }

    public char[] toCharArray() {

        return str().toCharArray();
    }

    public int length(){

        return SIZE;
    }
}
