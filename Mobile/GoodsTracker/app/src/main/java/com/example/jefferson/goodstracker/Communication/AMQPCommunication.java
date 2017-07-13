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
    public void deInit() {

        rabbitMQ.close();
    }

    @Override
    public boolean connection() {

        return rabbitMQ.connect();
    }

    @Override
    public void doPublish() {

        DataFrame frame = new DataFrame();

        if(DecoderFrame.cmd2Frame(takeFirstCmd(),frame)){

            rabbitMQ.publish(frame);
        }
    }

    @Override
    public void doSubscribe() {

        DataFrame frame = new DataFrame();
        AnsCmd      ans = new AnsCmd();

        if(DecoderFrame.frame2Ans(frame,ans)){

            acceptAnswer(ans);
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
        rabbitMQ.createExchange();
        rabbitMQ.createSubscribe(observer.getAddress());
    }
}
