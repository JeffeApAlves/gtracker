package com.example.jefferson.goodstracker.Communication;

import android.provider.ContactsContract;

import com.example.jefferson.goodstracker.RabbitMQ.RabbitMQ;

import java.io.IOException;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class AMQPCommunication extends Communication{

    RabbitMQ rabbitMQ = new RabbitMQ();

    protected AMQPCommunication(){

        rabbitMQ.open();

        startCommunication();
    }

    @Override
    public void deInit() {

        rabbitMQ.close();
    }

    @Override
    public void doPublish() {

//        rabbitMQ.publish();
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

        DataFrame frame = new DataFrame();

        if(DecoderFrame.cmd2Frame(cmd,frame)){

            rabbitMQ.publish(frame);
        }
    }

    @Override
    public void publishAnswer(AnsCmd ans) {

        DataFrame frame = new DataFrame();

        if(DecoderFrame.ans2Frame(ans,frame)){

            rabbitMQ.publish(frame);
        }
    }

    @Override
    public void registerObserver(ObserverCommunication observer) throws IOException {

        super.registerObserver(observer);
        rabbitMQ.create(observer.getAddress());
    }
}
