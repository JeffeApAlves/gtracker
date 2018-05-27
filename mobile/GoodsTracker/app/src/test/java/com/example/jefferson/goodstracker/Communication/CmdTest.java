package com.example.jefferson.goodstracker.Communication;

import android.graphics.YuvImage;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by Jefferson on 10/07/2017.
 */
public class CmdTest {

    Cmd     cmd;
    final   String expected = "EXEC_CALLBACK";

    @BeforeClass
    public static void onceExecutedBeforeAll() {

        Communication.create(TYPE_COMMUNICATION.AMQP);
    }

    @Before
    public void executedBeforeEach() {
/*
        cmd = new Cmd(1,2, RESOURCE_TYPE.LOCK, Operation.AN,new EventReceiveAnswer(){

            @Override
            public void onReceiveAnswer(AnsCmd ans){

                //Call back que sera usado como teste
                ans.getHeader().setData(expected);
            };
        });*/
    }

    @Test
    public void append() throws Exception {

        String expected     =   "1:2:3";

        cmd.append("1" + CONST_COM.CHAR.SEPARATOR);
        cmd.append("2" + CONST_COM.CHAR.SEPARATOR);
        cmd.append("3");

        assertEquals(expected,cmd.getPayload().getData());
    }

    @Test
    public void getHeader() throws Exception {

    }

    @Test
    public void setHeader() throws Exception {

    }

     @Test
    public void getPayload() throws Exception {

    }

    @Test
    public void setPayload() throws Exception {

    }

    @Test
    public void getResource() throws Exception {

    }

    @Test
    public void setAddress() throws Exception {

    }

    @Test
    public void setDest() throws Exception {

    }

    @Test
    public void getAddress() throws Exception {

    }

    @Test
    public void updateAnswer() throws Exception {
/*
        AnsCmd ans = new AnsCmd();
        ans.getHeader().setData("");

        cmd.updateAnswer(ans);

        assertEquals(expected,ans.getHeader().getData());*/
    }
}