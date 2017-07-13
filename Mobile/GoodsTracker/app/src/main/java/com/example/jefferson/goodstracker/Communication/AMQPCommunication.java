package com.example.jefferson.goodstracker.Communication;

import com.example.jefferson.goodstracker.RabbitMQ.RabbitMQ;
import java.io.IOException;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class AMQPCommunication extends Communication{

    RabbitMQ rabbitMQ= new RabbitMQ();

    protected AMQPCommunication(){

        rabbitMQ.open(getHandlerSubscribe());

        startCommunication();
    }

    @Override
    public void close() {

        rabbitMQ.close();
    }

    @Override
    public boolean connection() {

        return rabbitMQ.connect();
    }

    @Override
    public void publish(DataFrame frame) {

        rabbitMQ.publish(frame);
    }

    @Override
    public void registerObserver(ObserverCommunication observer) throws IOException {

        super.registerObserver(observer);

        //Declare all things
        rabbitMQ.createExchange();
        rabbitMQ.createSubscribe(observer.getAddress());
    }
}
