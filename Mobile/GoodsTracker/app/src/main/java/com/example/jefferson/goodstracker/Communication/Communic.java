package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface Communic {

    void init();
    void deInit();
    void doCommunication();
    public void registerObserver(ObserverAnswerCmd observer);
    public void removeObserver(ObserverAnswerCmd observer);
    void publishAnswer(DataFrame frame);
}
