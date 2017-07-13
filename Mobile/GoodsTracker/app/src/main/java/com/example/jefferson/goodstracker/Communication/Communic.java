package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface Communic {

    boolean connect();
    void close();
    void startWorkerPublish();
    void startWorkerSubscribe();
    void publish(DataFrame frame);
    void subscribe(DataFrame frame);
}
