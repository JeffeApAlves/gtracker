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

    private ConnectionFactory   factory         = null;
    private Connection          connection      = null;
    private Channel             channel         = null;

    public void open(){

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

            channel.basicQos(1);

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
    public void publish(String exchange,String routing,DataFrame frame) {

        if(frame==null) return;

        try {

            channel.confirmSelect();

            try{
                channel.basicPublish(exchange, routing, null, frame.toBytesArray());
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

    public void createExchange()throws java.io.IOException {

        channel.exchangeDeclare(RABBITMQ_CONST.EXCHANGE.CMD, RABBITMQ_CONST.EXCHANGE.TYPE.TOPIC);
        channel.exchangeDeclare(RABBITMQ_CONST.EXCHANGE.ANS, RABBITMQ_CONST.EXCHANGE.TYPE.TOPIC);
    }

    /**
     *
     *
     */
    public void createConsumerCmd(int address,Handler handler) {

        try {

            ConsumerCmd consumerCmd = new ConsumerCmd(address);
            consumerCmd.setHandler(handler);

        } catch (Exception e) {

            Log.d("","Problema na criacao dos consumidores: " + e.getClass().getName());
        }
    }

    /**
     *
     *
     */
    public void createConsumerAns(int address,Handler handler) {

        try {

            ConsumerAns consumerAns = new ConsumerAns(address);
            consumerAns.setHandler(handler);

        } catch (Exception e) {

            Log.d("","Problema na criacao dos consumidores: " + e.getClass().getName());
        }
    }

    class ConsumerAns extends DefaultConsumer{

        private Handler handler = null;

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

        public void setHandler(Handler handler) {
            this.handler = handler;
        }
    }

    class ConsumerCmd extends DefaultConsumer{

        private Handler handler = null;

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

        public void setHandler(Handler handler) {
            this.handler = handler;
        }
    }
}

//Raimundo
//031 37625426