package com.example.jefferson.goodstracker.Communication;

import java.io.IOException;

/**
 * Created by Jefferson on 09/07/2017.
 */

public interface ObservableCommunication{

    /**
     * Register
     *
     * @param observer
     */
    void registerObserver(ObserverCommunication observer) throws IOException;

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
