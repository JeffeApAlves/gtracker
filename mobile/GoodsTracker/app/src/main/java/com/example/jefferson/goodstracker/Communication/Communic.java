package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface Communic {

    boolean connect();
    void close();
    void startWorkerProducer();
    void startWorkerConsumer();
    void producerFrame(DataFrame frame);
    void consumerFrame(DataFrame frame);
}
