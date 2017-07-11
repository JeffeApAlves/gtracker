package com.example.jefferson.goodstracker.Communication;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.TimeoutException;

/**
 * Created by Jefferson on 07/07/2017.
 */

public class RabbitMQ {

    private ConnectionFactory   factory     = null;
    private Connection          connection  = null;
    private Channel             channel     = null;
    Handler                     handler     = null;

    private BlockingDeque<String> queue     = new LinkedBlockingDeque<String>();

    public void open(){

        factory = new ConnectionFactory();

        factory.setHost(RABBITMQ_CONST.HOST);
        factory.setVirtualHost(RABBITMQ_CONST.VHOST);
        factory.setUsername(RABBITMQ_CONST.USER);
        factory.setPassword(RABBITMQ_CONST.PW);
        factory.setPort(RABBITMQ_CONST.PORT);
    }

    public void close(){

        try {

            channel.close();
            connection.close();

        }catch (IOException e) {

            e.printStackTrace();
            Log.d("", "Problema na finalizacao da conexao: " + e.getClass().getName());

        }catch (TimeoutException e1) {

            e1.printStackTrace();
            Log.d("", "Problema na finalizacao da conexao: " + e1.getClass().getName());
        }
    }

    public void putLast(String msg){

        try{

            Log.d("","[q] " + msg);
            queue.putLast(msg);

        }catch (InterruptedException e) {

            e.printStackTrace();
            Log.d("", "Connection broken: " + e.getClass().getName());
        }
    }

    /**
     *
     *
     * @return
     */
    boolean connect() {

        boolean ret;

        try {

            connection  = factory.newConnection();
            channel     = connection.createChannel();

            ret = true;
        } catch (IOException e) {

            Log.d("", "Nao foi possivel conectar: " + e.getClass().getName());
            e.printStackTrace();
            ret = false;

        } catch (TimeoutException e1) {

            Log.d("", "Nao foi possivel conectar: " + e1.getClass().getName());
            e1.printStackTrace();
            ret = false;
        }

        return ret;
    }

    /**
     *
     *
     */
    public void publish() {

        while(true) {

            try {
                channel.confirmSelect();

                while (true) {
                    String message = queue.takeFirst();
                    try{
                        channel.basicPublish("amq.fanout", "chat", null, message.getBytes());
                        channel.waitForConfirmsOrDie();
                        Log.d("", "[s] " + message);
                    } catch (Exception e){
                        Log.d("","[f] " + message);
                        queue.putFirst(message);
                        throw e;
                    }
                }
            } catch (InterruptedException e) {
                break;
            } catch (Exception e) {
                Log.d("","Problema na transmissao: " + e.getClass().getName());
            }
        }
    }

    /**
     *
     *
     */
    public void subscribe() {

        while(true) {

            try {
                channel.basicQos(1);
                AMQP.Queue.DeclareOk q = channel.queueDeclare();
                channel.queueBind(q.getQueue(), "amq.fanout", "chat");
                QueueingConsumer consumer = new QueueingConsumer(channel);
                channel.basicConsume(q.getQueue(), true, consumer);

                while (true) {

                    QueueingConsumer.Delivery delivery = consumer.nextDelivery();
                    String message = new String(delivery.getBody());
                    Log.d("","[r] " + message);

                    if(handler!=null) {

                        Message msg = handler.obtainMessage();

                        Bundle bundle = new Bundle();
                        bundle.putString("msg", message);
                        msg.setData(bundle);

                        handler.sendMessage(msg);
                    }
                }
            } catch (InterruptedException e) {
                break;
            } catch (Exception e1) {
                Log.d("","Problema na recepcao: " + e1.getClass().getName());
            }
        }
    }
}

//Raimundo
//031 37625426