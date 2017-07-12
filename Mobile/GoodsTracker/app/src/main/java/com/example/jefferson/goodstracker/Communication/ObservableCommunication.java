package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 09/07/2017.
 */

public interface ObservableCommunication{

    /**
     * Register
     *
     * @param observer
     */
    void registerObserver(ObserverCommunication observer);

    /**
     * Unregister
     *
     * @param observer
     */
    void removeObserver(ObserverCommunication observer);

    /*
     * Notifica apenas a entidade respectiva do tracker correspondente
     */
    void notifyObserver(Object ans);
}
