package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface Communic {

//    void init();
    void deInit();
    void doPublish();
    void doSubscribe();
    boolean connection();
    void publishCmd(Cmd cmd);
    void publishAnswer(AnsCmd ans);
    void acceptAnswer(AnsCmd ans);
}
