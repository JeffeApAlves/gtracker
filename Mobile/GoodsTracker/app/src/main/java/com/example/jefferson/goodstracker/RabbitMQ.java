package com.example.jefferson.goodstracker;

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

    static final String HOST    = "192.168.0.108";
    static final String VHOST   = "/";
    static final String USER    = "rna";
    static final String PW      = "rna@1981";
    static final int    PORT    = 5672;

    ConnectionFactory   factory;
    Connection          connection= null;
    Channel             channel;

    Thread              subscribeThread,
                        publishThread;

    private BlockingDeque<String> queue = new LinkedBlockingDeque<String>();

    public void init(){

        factory = new ConnectionFactory();

        factory.setAutomaticRecoveryEnabled(false);
        factory.setHost(HOST);
        factory.setVirtualHost(VHOST);
        factory.setUsername(USER);
        factory.setPassword((PW));
        factory.setPort(PORT);

        publishToAMQP();
    }

    public void deInit(){

        publishThread.interrupt();
        subscribeThread.interrupt();

        try {

            channel.close();
            connection.close();

        }catch (IOException e) {

            e.printStackTrace();
            Log.d("", "Connection broken: " + e.getClass().getName());

        }catch (TimeoutException e1) {

            e1.printStackTrace();
            Log.d("", "Connection broken: " + e1.getClass().getName());
        }
    }

    public void connect(){

        if(connection==null) {

            try {
                ConnectionFactory factory = new ConnectionFactory();

                factory.setAutomaticRecoveryEnabled(false);
                factory.setHost(HOST);
                factory.setVirtualHost(VHOST);
                factory.setUsername(USER);
                factory.setPassword((PW));
                factory.setPort(PORT);

                if (factory != null) {

                    connection  = factory.newConnection();
                    channel     = connection.createChannel();
                }

            } catch (IOException e) {

                e.printStackTrace();
                Log.d("", "Connection broken: " + e.getClass().getName());

            } catch (TimeoutException e1) {

                e1.printStackTrace();
                Log.d("", "Connection broken: " + e1.getClass().getName());
            }
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

    public void publishToAMQP() {

        publishThread = new Thread(new Runnable() {
            @Override
            public void run() {

            connect();
            while(true) {
                try {
                    channel.confirmSelect();

                    while (true) {
                        String message = queue.takeFirst();
                        try{
                            channel.basicPublish("amq.fanout", "chat", null, message.getBytes());
                            Log.d("", "[s] " + message);
                            channel.waitForConfirmsOrDie();
                        } catch (Exception e){
                            Log.d("","[f] " + message);
                            queue.putFirst(message);
                            throw e;
                        }
                    }
                } catch (InterruptedException e) {
                    break;
                } catch (Exception e) {
                    Log.d("", "Connection broken: " + e.getClass().getName());
                    try {
                        Thread.sleep(5000); //sleep and then try again
                    } catch (InterruptedException e1) {
                        break;
                    }
                }
            }
            }
        });

        publishThread.start();
    }

    void subscribe(final Handler handler) {

        subscribeThread = new Thread(new Runnable() {
            @Override
            public void run() {

            connect();
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
                        Message msg = handler.obtainMessage();
                        Bundle bundle = new Bundle();
                        bundle.putString("msg", message);
                        msg.setData(bundle);
                        handler.sendMessage(msg);
                    }
                } catch (InterruptedException e) {
                    break;
                } catch (Exception e1) {
                    Log.d("", "Connection broken: " + e1.getClass().getName());
                    try {
                        Thread.sleep(5000); //sleep and then try again
                    } catch (InterruptedException e) {
                        break;
                    }
                }
            }
            }
        });

        subscribeThread.start();
    }
}