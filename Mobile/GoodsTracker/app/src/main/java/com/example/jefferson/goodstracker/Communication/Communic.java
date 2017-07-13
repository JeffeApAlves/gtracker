package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface Communic {

    boolean connection();
    void close();
    void startWorkerPublish();
    void startWorkerSubscribe();
    void publish(DataFrame frame);
    void acceptAnswer(AnsCmd ans);
}
