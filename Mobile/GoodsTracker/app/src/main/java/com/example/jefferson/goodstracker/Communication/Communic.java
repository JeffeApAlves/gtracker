package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface Communic {

    void Init();
    void DeInit();
    void doCommunication();
    void publishAnswer(DataFrame frame);
}
