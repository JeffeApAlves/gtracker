package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 09/07/2017.
 */

public interface ObserverCommunication {

    public int getAddress();
    public void connEstablished();
    public void updateCommunication(Object obj);
}
