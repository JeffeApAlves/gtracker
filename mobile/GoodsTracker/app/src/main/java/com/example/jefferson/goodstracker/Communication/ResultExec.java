package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public enum ResultExec {

    EXEC_UNSUCCESS (-3),
    INVALID_CMD(-2),
    INVALID_PARAM(-1),
    EXEC_SUCCESS(0),;

    private final int valor;

    ResultExec(int valor) {
        this.valor = valor;
    }
}
