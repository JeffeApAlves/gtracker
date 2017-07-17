package com.example.jefferson.goodstracker.RabbitMQ;

import com.example.jefferson.goodstracker.Communication.ChatMessage;
import com.example.jefferson.goodstracker.Communication.Communication;
import com.example.jefferson.goodstracker.Communication.DataFrame;
import com.example.jefferson.goodstracker.Communication.ObserverCommunication;
import java.io.IOException;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class AMQPCommunication extends Communication {

    RabbitMQ rabbitMQ = new RabbitMQ();

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
    public void producerFrame(ChatMessage message) {

        String routing = "chat." + message.getDest();

        rabbitMQ.publish(RABBITMQ_CONST.EXCHANGE.CHAT, routing, message.getBody());
    }

    @Override
    public void producerFrame(DataFrame frame) {

        String routing = "cmd." + String.format("%05d", frame.getHeader().getDest());

        rabbitMQ.publish(RABBITMQ_CONST.EXCHANGE.CMD, routing, frame);
    }

    @Override
    public void registerObserver(ObserverCommunication observer) throws IOException {

        super.registerObserver(observer);

        // Declare all things inside of server rabbitmq
        rabbitMQ.createExchange();
        rabbitMQ.createAllConsumers(observer.getAddress(), getHandlerConsumer());
    }

    @Override
    public void notifyConnEstablished() {

        // Criando consumer e definindo handler de manipulacao das mensagens recebidas para cada unidade
        for (ObserverCommunication observer : getArrayOfUnit()) {

            rabbitMQ.createAllConsumers(observer.getAddress(), getHandlerConsumer());

            observer.connEstablished();
        }
    }
}