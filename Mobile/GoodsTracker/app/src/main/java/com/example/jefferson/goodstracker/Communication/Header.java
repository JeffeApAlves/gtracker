package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class Header  extends Object{

    public final int LENGTH = 28;             // (5)+(5)+(5)+(2)+(3)+(3) + 5 separadores

    private int         dest;
    private int         address;
    private int         count = 0;
    private Operation   operation;
    private String      resource;
    private int         sizePayLoad;

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

        return Decoder.header2str(this);
    }

    public void clear() {

    }

    public char[] toCharArray() {

        return str().toCharArray();
    }

    public int length(){

        return LENGTH;
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
