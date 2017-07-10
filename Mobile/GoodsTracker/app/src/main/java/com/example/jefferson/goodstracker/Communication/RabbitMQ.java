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
    private Channel             channel;

    private Thread              connThread,
                                subscribeThread,
                                publishThread;

    private BlockingDeque<String> queue = new LinkedBlockingDeque<String>();

    public void open(){

        factory = new ConnectionFactory();

        factory.setHost(RABBITMQ_CONST.HOST);
        factory.setVirtualHost(RABBITMQ_CONST.VHOST);
        factory.setUsername(RABBITMQ_CONST.USER);
        factory.setPassword(RABBITMQ_CONST.PW);
        factory.setPort(RABBITMQ_CONST.PORT);

        createConnThread();
    }

    public void close(){

        connThread.interrupt();
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

    public void putLast(String msg){

        try{

            Log.d("","[q] " + msg);
            queue.putLast(msg);

        }catch (InterruptedException e) {

            e.printStackTrace();
            Log.d("", "Connection broken: " + e.getClass().getName());
        }
    }

    public void connect() throws InterruptedException {

        connThread.start();

        Thread.sleep(1000);

        subscribeThread.start();
        publishThread.start();
    }

    public void createConnThread(){

        connThread = new Thread(new Runnable() {

            @Override
            public void run() {

                synchronized (connThread) {

                    try {

                        if (factory != null) {

                            connection = factory.newConnection();
                            channel = connection.createChannel();

                            connThread.notifyAll();
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
        });
    }

    public void createPublishThread() {

        publishThread = new Thread(new Runnable() {

            @Override
            public void run() {

                // Espera a conexao
                synchronized (connThread){

                    try {

                        connThread.wait();

                    } catch (InterruptedException e) {

                        e.printStackTrace();
                    }
                }

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
    }

    public void createSubscribeThread(final Handler handler) {

        subscribeThread = new Thread(new Runnable() {

            @Override
            public void run() {

                // Espera a conexao
                synchronized (connThread){

                    try {

                        connThread.wait();

                    } catch (InterruptedException e) {

                        e.printStackTrace();
                    }
                }

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
    }
}