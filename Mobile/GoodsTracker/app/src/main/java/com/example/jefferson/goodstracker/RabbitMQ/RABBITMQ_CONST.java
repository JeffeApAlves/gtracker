package com.example.jefferson.goodstracker.RabbitMQ;

/**
 * Created by Jefferson on 07/07/2017.
 */

public class RABBITMQ_CONST  extends Object{

    public static final String HOSTNAME= "192.168.0.113";
    public static final String VHOST   = "/";
    public static final String USER    = "mt5";
    public static final String PW      = "mt5@1981";
    public static final int    PORT    = 5672;


    public class EXCHANGE {

        public static final String ANS    = "ANS";
        public static final String CMD    = "CMD";

        public class TYPE{

            public static  final String DIRECT  = "direct";
            public static  final String TOPIC   = "topic";
        }
    }

    public class QUEUE{

        public static final String CMD       = "CMD";
        public static final String TLM       = "TLM";
    }
}
