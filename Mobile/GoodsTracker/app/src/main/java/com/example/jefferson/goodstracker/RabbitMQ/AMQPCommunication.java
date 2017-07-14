package com.example.jefferson.goodstracker.RabbitMQ;

import com.example.jefferson.goodstracker.Communication.Communication;
import com.example.jefferson.goodstracker.Communication.DataFrame;
import com.example.jefferson.goodstracker.Communication.ObserverCommunication;
import java.io.IOException;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class AMQPCommunication extends Communication {

    RabbitMQ rabbitMQ = new RabbitMQ();
/*
    public AMQPCommunication() {

    }*/

    @Override
    public void close() {

        super.close();
        rabbitMQ.close();
    }

    @Override
    public boolean connect() {

        rabbitMQ.open();

        return rabbitMQ.connect();
    }

    @Override
    public void producerFrame(DataFrame frame) {

        String routing = "cmd." + String.format("%05d",frame.getHeader().getDest());

        rabbitMQ.publish(RABBITMQ_CONST.EXCHANGE.CMD,routing,frame);
    }

    @Override
    public void registerObserver(ObserverCommunication observer) throws IOException {

        super.registerObserver(observer);

        // Declare all things inside of server rabbitmq
        rabbitMQ.createExchange();
    }

    @Override
    public void startWorkerConsumer() {

        super.startWorkerConsumer();

        // Criando consumer e definindo handler de manipulacao das mensagens recebidas para cada unidade
        for(ObserverCommunication ob:getArrayOfUnit()){

            rabbitMQ.createConsumerAns(ob.getAddress(),getHandlerConsumer());
            rabbitMQ.createConsumerCmd(ob.getAddress(),getHandlerConsumer());
        }
    }
}