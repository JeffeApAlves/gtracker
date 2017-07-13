package com.example.jefferson.goodstracker.RabbitMQ;

import android.util.Log;

import com.example.jefferson.goodstracker.Communication.DataFrame;
import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * Created by Jefferson on 07/07/2017.
 */

public class RabbitMQ  extends Object {

    private ConnectionFactory   factory         = null;
    private Connection          connection      = null;
    private Channel             channel         = null;


    public void open(){

        factory = new ConnectionFactory();

        factory.setHost(RABBITMQ_CONST.HOSTNAME);
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

    /**
     *
     *
     * @return
     */
    public boolean connect() {

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
    public void publish(DataFrame frame) {

        if(frame==null) return;

        try {
            channel.confirmSelect();

            try{
                channel.basicPublish("amq.fanout", "chat", null, frame.getBytes());
                channel.waitForConfirmsOrDie();
                Log.d("", "[sucesso] " + frame.str());
            } catch (Exception e){
                Log.d("","[fail] " + frame.str());

                throw e;
            }
        } catch (InterruptedException e) {
            Log.d("","Problema na transmissao: " + e.getClass().getName());
        } catch (Exception e) {
            Log.d("","Problema na transmissao: " + e.getClass().getName());
        }
    }

    /**
     *
     *
     */
    public void subscribe() {

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

                //TODO Publicar mensagens
/*
                if(handler!=null) {

                    Message msg = handler.obtainMessage();

                    Bundle bundle = new Bundle();
                    bundle.putString("msg", message);
                    msg.setData(bundle);

                    handler.sendMessage(msg);
                }*/
            }
        } catch (InterruptedException e) {
            Log.d("","Problema na recepcao: " + e.getClass().getName());
        } catch (Exception e1) {
            Log.d("","Problema na recepcao: " + e1.getClass().getName());
        }
    }

    public void create(int address)throws java.io.IOException {

        String queueCmd = RABBITMQ_CONST.EXCHANGE.CMD+String.format("%05d",address);
        String queueAns = RABBITMQ_CONST.EXCHANGE.ANS+String.format("%05d",address);

        channel.exchangeDeclare(RABBITMQ_CONST.EXCHANGE.CMD,RABBITMQ_CONST.DIRECT);
        channel.exchangeDeclare(RABBITMQ_CONST.EXCHANGE.ANS,RABBITMQ_CONST.DIRECT);

        channel.queueDeclare(queueCmd,false,false,false,null);
        channel.queueDeclare(queueAns,false,false,false,null);

        channel.queueBind(  queueCmd,
                            RABBITMQ_CONST.EXCHANGE.CMD,
                            "R"+String.format("%05d",address));

        channel.queueBind(  queueAns,
                            RABBITMQ_CONST.EXCHANGE.ANS,
                            "R"+String.format("%05d",address));
    }
}

//Raimundo
//031 37625426