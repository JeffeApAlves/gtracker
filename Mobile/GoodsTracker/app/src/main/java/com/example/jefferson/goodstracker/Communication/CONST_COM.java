package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class CONST_COM  extends Object{

    public class CONFIG{

        static public final int TIME_COMMUNICATION = 10;
    }

    public class CHAR {

        static public final char RX_FRAME_START    = '[';
        static public final char RX_FRAME_END      = ']';
        static public final char CR                = '\r';
        static public final char LF                = '\n';
        static public final char SEPARATOR         = ':';
        static public final char ASTERISCO         = '*';
    }

    public  class MASTER{

        static  public  final  int ADDRESS          = 5;
    }
}
