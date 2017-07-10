package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class DataFrame {

    protected Header    header;
    protected PayLoad   payLoad;
    protected String    data;

    public DataFrame(String data) {

        setData(data);
    }

    public DataFrame(Cmd cmd) {

        data    = "";
        setHeader(  cmd.getHeader());
        setPayLoad( cmd.getPayload());
    }

    public DataFrame() {

        header  = new Header();
        payLoad = new PayLoad();
        data   = "";
    }

    Header getHeader() {

        return header;
    }

    void setHeader(Header value) {

        header  = value;
        data    = header.str() + CONST_COM.CHAR.SEPARATOR + (payLoad==null?"":payLoad.str());
    }

    PayLoad getPayLoad() {

        return payLoad;
    }

    void setPayLoad(PayLoad value) {

        payLoad = value;
        data    = header.str() + CONST_COM.CHAR.SEPARATOR + (payLoad == null ? "" : payLoad.str());
    }

    String getData() {

        return data;
    }

    void setData(String value) {

        data   = value;

        if(value.length() >= header.length()+1){

            header.setData(value.substring(0, header.length()));
            payLoad.setData(value.substring( (header.length() + 1), value.length()));
        }else{

            header.setData(value.substring(0, value.length()));
            payLoad.setData("");
        }
    }

    public byte getByte(int i) {

        return data.getBytes()[i];
    }

    public void putByte(char b) {

        data += b;
    }

    public int getSizeOfFrame() {

        return data==null?0:data.length();
    }

    public boolean isFrameEmpty() {

        return getSizeOfFrame() <= 0;
    }

    public byte checkSum() {

        byte checkSum = 0;

        byte[] datas = data.getBytes();

        for (int i = 0; i < datas.length; i++) {

            checkSum ^= datas[i];
        }

        return checkSum;
    }

    public String str() {

        //TODO verificar a conversao para hexa 2 digitos

        return  CONST_COM.CHAR.RX_FRAME_START +
                data +
                Integer.toHexString(checkSum()) +
                CONST_COM.CHAR.RX_FRAME_END;
    }

    public char[] toCharArray() {

        return  str().toCharArray();
    }
}
