package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 09/07/2017.
 */

public interface ObservableAnswerCmd {

    public void registerObserver(ObserverAnswerCmd observer);
    public void removeObserver(ObserverAnswerCmd observer);
    public void notifyAllObservers(AnsCmd ans);
    public void notifyObserver(AnsCmd ans);
}
