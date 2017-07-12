package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class AMQPCommunication extends Communication{

    RabbitMQ rabbitMQ = new RabbitMQ();

    @Override
    public void init() {

        rabbitMQ.open();

        super.init();
    }

    @Override
    public void doPublish() {

        rabbitMQ.publish();
    }

    @Override
    public void doSubscribe(){

        rabbitMQ.subscribe();
    }

    @Override
    public boolean connection() {

        return rabbitMQ.connect();
    }

    @Override
    public void publishCmd(Cmd cmd) {

    }

    public void publishAnswer(DataFrame frame) {

    }
}
