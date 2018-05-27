package com.example.jefferson.goodstracker.Domain;

/**
 * Created by Jefferson on 17/07/2017.
 */

public class Notification {

    enum type_notification{

        OK,
        WARNING,
        DANGER,
    }

    type_notification   type;
    String              message;

    Notification(String msg,type_notification type){

        this.message    = msg;
        this.type       = type;
    }

    public type_notification getType() {
        return type;
    }

    public String getMessage() {
        return message;
    }
}
