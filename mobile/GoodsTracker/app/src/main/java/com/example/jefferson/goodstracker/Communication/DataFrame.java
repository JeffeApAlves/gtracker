package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class DataFrame  extends Object {

    protected   TypeFrame       typeFrame;
    protected   FormatFrame     formatFrame;
    protected   Header          header;
    protected   PayLoad         payload;
    protected   String          data;

    public DataFrame(FormatFrame format,TypeFrame type,String data) {

        this.typeFrame      = type;
        this.formatFrame    = format;
        setData(data);
    }

    public DataFrame(FormatFrame format,TypeFrame type) {

        this.formatFrame    = format;
        header              = new Header();
        payload             = new PayLoad();
        data                = "";
    }

    public Header getHeader() {

        return header;
    }

    public void setHeader(Header value) {

        header              = value;
        updateData();
    }

    PayLoad getPayLoad() {

        return payload;
    }

    void setPayLoad(PayLoad value) {

        payload = value;
        updateData();
    }

    String getData() {

        return data;
    }

    private void updateData(){

        IDecoder decoder    = DecoderFrame.create(formatFrame);

        data    =           decoder.header_to_str(header)           +
                            CONST_COM.CHAR.SEPARATOR                +
                            String.format("%03d", payload.length())  +
                            CONST_COM.CHAR.SEPARATOR                +
                            payload.getData()                       +
                            CONST_COM.CHAR.SEPARATOR;
    }

    private void updateHeaderPayLoad(){

        final int HEADER_LENGTH = 27;             // (5)+(5)+(5)+(2)+(3)+(3) + 5 separadores

        if(data.length() >= HEADER_LENGTH+2){

            IDecoder decoder = DecoderFrame.create(formatFrame);

            header  = decoder.str_to_header(data);

            //+2 -3 ja para eliminar os separadores
            payload = new PayLoad(data.substring( (HEADER_LENGTH + 2), data.length()-3));

        }else{

            header  = new Header();
            payload = new PayLoad();
        }
    }

    void setData(String value) {

        data = value;

        updateHeaderPayLoad();
    }

    public byte getByte(int i) {

        return data.getBytes()[i];
    }

    public void append(char b) {

        data +=b;
    }

    public int getSizeOfFrame() {

        return data==null?0:data.length();
    }

    public boolean isFrameEmpty() {

        return getSizeOfFrame() <= 0;
    }

    public byte calcSum() {

        byte checkSum = 0;

        byte[] d = data.getBytes();

        for (int i = 0; i < getSizeOfFrame(); i++) {

            checkSum ^= d[i];
        }

        return checkSum;
    }

    public boolean checkSum(byte val) {

        return calcSum()==val;
    }

    public FormatFrame getFormatFrame() {

        return formatFrame;
    }

    @Override
    public String toString() {

        //TODO verificar a conversao para hexa 2 digitos

        return  /*CONST_COM.CHAR.RX_FRAME_START +*/
                data.toString()                 +
                Integer.toHexString(calcSum())  /*+
                CONST_COM.CHAR.RX_FRAME_END*/;
    }

    public char[] toCharArray() {

        return  toString().toCharArray();
    }

    public byte[] toBytesArray() {

        return  toString().getBytes();
    }

    public void setFormatFrame(FormatFrame formatFrame) {
        this.formatFrame = formatFrame;
    }

    public TypeFrame getTypeFrame() {
        return typeFrame;
    }

    public void setTypeFrame(TypeFrame typeFrame) {
        this.typeFrame = typeFrame;
    }
}
