package com.example.jefferson.goodstracker.Communication;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by Jefferson on 10/07/2017.
 */
public class DataFrameTest {

    DataFrame frame;

    @BeforeClass
    public static void onceExecutedBeforeAll() {

        Communication.create(TYPE_COMMUNICATION.AMQP);
    }

    @Before
    public void executedBeforeEach() {

        frame = new DataFrame(TypeFrame.OWNER);
    }

    @Test
    public void getHeader() throws Exception {

    }

    @Test
    public void setHeader() throws Exception {

    }

    @Test
    public void getPayLoad() throws Exception {

    }

    @Test
    public void setPayLoad() throws Exception {

    }

    @Test
    public void getData() throws Exception {

    }

    @Test
    public void setData() throws Exception {

        frame.setData("00001:00002:00005:AN:TLM:007:1:2:3:4");

        Header  header  = frame.getHeader();
        PayLoad payLoad = frame.getPayload();

        assertEquals(1,header.getAddress());
        assertEquals(2,header.getDest());
        assertEquals(5,header.getCount());
        assertEquals(Operation.AN,header.getOperation());
        assertEquals("TLM",header.getResource());
        assertEquals(7,header.getSizePayLoad());

        assertEquals("1:2:3:4",payLoad.getData());
    }

    @Test
    public void getByte() throws Exception {

    }

    @Test
    public void putByte() throws Exception {

    }

    @Test
    public void getSizeOfFrame() throws Exception {

    }

    @Test
    public void isFrameEmpty() throws Exception {

    }

    @Test
    public void checkSum() throws Exception {

    }

    @Test
    public void str() throws Exception {

    }

    @Test
    public void toCharArray() throws Exception {

    }
}