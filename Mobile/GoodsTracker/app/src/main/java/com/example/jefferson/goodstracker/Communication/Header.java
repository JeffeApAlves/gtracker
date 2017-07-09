package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Header {

    public final int LENGTH = 27;             // 5+5+5+2+3+3 + 4 separadores

    String      data;
    int         dest;
    int         address;
    int         count = 0;
    Operation   operation;
    String      resource;
    int         sizePayLoad;

    public Header(String r, Operation o) {

        count       = 0;
        address     = 0;
        dest        = 0;
        resource    = r;
        operation   = o;
        sizePayLoad = 0;
    }

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

        if (value.length() > LENGTH) {

            data = value.substring(0, LENGTH);

        } else {

            data = value;
        }
    }

    public char[] toCharArray() {

        return str().toCharArray();
    }

    public int length(){

        return LENGTH;
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public int getDest() {
        return dest;
    }

    public void setDest(int dest) {
        this.dest = dest;
    }

    public int getAddress() {
        return address;
    }

    public void setAddress(int address) {
        this.address = address;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public Operation getOperation() {
        return operation;
    }

    public void setOperation(Operation operation) {
        this.operation = operation;
    }

    public String getResource() {
        return resource;
    }

    public void setResource(String resource) {
        this.resource = resource;
    }

    public int getSizePayLoad() {
        return sizePayLoad;
    }

    public void setSizePayLoad(int sizePayLoad) {
        this.sizePayLoad = sizePayLoad;
    }
}
