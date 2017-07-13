package com.example.jefferson.goodstracker.RabbitMQ;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.example.jefferson.goodstracker.Communication.DataFrame;
import com.example.jefferson.goodstracker.Communication.IdMessage;
import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * Created by Jefferson on 07/07/2017.
 */

public class RabbitMQ  extends Object {

    private Handler             handler         = null;
    private ConnectionFactory   factory         = null;
    private Connection          connection      = null;
    private Channel             channel         = null;

    public void open(Handler handler){

        this.handler    = handler;

        factory         = new ConnectionFactory();

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
    public void createSubscribe(int address) {

        try {

            channel.basicQos(1);

            Consumer consumerCmd = new ConsumerCmd(address);
            Consumer consumerAns = new ConsumerAns(address);

        } catch (Exception e) {

            Log.d("","Problema na recepcao: " + e.getClass().getName());
        }
    }

    class ConsumerAns extends DefaultConsumer{

        public ConsumerAns(int address) throws IOException {

            super(channel);

            String str_address  = String.format("%05d",address);
            String queue        = RABBITMQ_CONST.EXCHANGE.ANS+str_address;
            String route        = "ans."+ str_address;

            channel.queueDeclare(queue,false,false,false,null);
            channel.queueBind(queue, RABBITMQ_CONST.EXCHANGE.ANS, route);
            channel.basicConsume(queue, true, this);
        }

        @Override
        public void handleDelivery(String consumerTag, Envelope envelope,
                                   AMQP.BasicProperties properties, byte[] body) throws IOException {

            String message = new String(body, "UTF-8");

            Log.d("","[ans] " + message);

            if(handler!=null) {

                Message msg = handler.obtainMessage();

                msg.what    = IdMessage.ANS.ordinal();

                Bundle bundle = new Bundle();
                bundle.putString("PAYLOAD", message);
                msg.setData(bundle);

                handler.sendMessage(msg);
            }
        }
    }

    class ConsumerCmd extends DefaultConsumer{

        public ConsumerCmd(int address) throws IOException {

            super(channel);
            String str_address  = String.format("%05d",address);
            String queue        = RABBITMQ_CONST.EXCHANGE.CMD+str_address;
            String route        = "cmd."+ str_address;

            channel.queueDeclare(   queue,  false,false,false,null);
            channel.queueBind(      queue, RABBITMQ_CONST.EXCHANGE.CMD, route);
            channel.basicConsume(   queue, true, this);
        }

        @Override
        public void handleDelivery(String consumerTag, Envelope envelope,
                                   AMQP.BasicProperties properties, byte[] body) throws IOException {

            String message = new String(body, "UTF-8");

            Log.d("","[cmd] " + message);

            if(handler!=null) {

                Message msg = handler.obtainMessage();

                msg.what = IdMessage.CMD.ordinal();

                Bundle bundle = new Bundle();
                bundle.putString("PAYLOAD", message);
                msg.setData(bundle);

                handler.sendMessage(msg);
            }
        }
    }

    public void createExchange()throws java.io.IOException {

        channel.exchangeDeclare(RABBITMQ_CONST.EXCHANGE.CMD, RABBITMQ_CONST.EXCHANGE.TYPE.TOPIC);
        channel.exchangeDeclare(RABBITMQ_CONST.EXCHANGE.ANS, RABBITMQ_CONST.EXCHANGE.TYPE.TOPIC);
    }
}

//Raimundo
//031 37625426