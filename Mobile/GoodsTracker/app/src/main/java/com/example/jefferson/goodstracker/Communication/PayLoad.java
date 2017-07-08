package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class PayLoad {

    public final int    LEN_MAX_PAYLOAD = 256;
    private String      data;


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


    public void append(double b)
    {
        //TODO verificar o que o parametro G faz na conversao string
        //data += b.toString("G");

        data += Double.toString(b);
    }

    public int length()
    {
        return data==null? 0:data.length();
    }

    public boolean IsFull()
    {
        return length() >= LEN_MAX_PAYLOAD;
    }

    public boolean IsEmpty()
    {
        return length() <= 0;
    }

    public String str()
    {
        return String.format("%0d",length()) + CONST_COM.CHAR.SEPARATOR + data + CONST_COM.CHAR.SEPARATOR;
    }

    public void Clear()
    {
        data = "";
    }

    public void setData(DataFrame frame) {

        String value = frame.getData();

        if (value.length() > (Header.SIZE + 1)) {

            data = value.substring((Header.SIZE + 1), value.length() - (Header.SIZE + 1));
        }
    }

    public char[] toCharArray() {

        return str().toCharArray();
    }
}
