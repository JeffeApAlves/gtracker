package com.example.jefferson.goodstracker.Communication;

import com.example.jefferson.goodstracker.Domain.Tol;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import java.util.Arrays;

import static org.junit.Assert.*;

/**
 * Created by Jefferson on 10/07/2017.
 */
public class PayLoadTest {

    PayLoad payload;

    @BeforeClass
    public static void onceExecutedBeforeAll() {

        Communication.create(TYPE_COMMUNICATION.AMQP);
    }

    @Before
    public void executedBeforeEach() {

        payload = new PayLoad();
    }

    @Test
    public void appendBoolean() throws Exception {

        String expected = "1";

        payload.append(true);

        assertEquals(expected,payload.getData());
    }

    @Test
    public void appendChar() throws Exception {

        String expected = ":";

        payload.append(':');

        assertEquals(expected,payload.getData());
    }

    @Test
    public void append2() throws Exception {

        String expected = "A:B";

        payload.append("A:B");

        assertEquals(expected,payload.getData());
    }

    @Test
    public void append3() throws Exception {

        String expected = "15.5";

        payload.append(15.5);

        assertEquals(expected,payload.getData());
    }

    @Test
    public void length() throws Exception {

        String str = "0123456789";
        int expected = str.length();

        payload.setData("0123456789");

        assertEquals(expected,payload.length());
    }

    @Test
    public void isFull() throws Exception {

        int n = 300;
        char[] chars = new char[n];
        Arrays.fill(chars, 'c');
        String result = new String(chars);

        payload.setData(result);
        assertTrue(payload.isFull());

        payload.clear();
        payload.setData("AAAAA");
        assertFalse(payload.isFull());
    }

    @Test
    public void isEmpty() throws Exception {

        payload.clear();
        assertTrue(payload.isEmpty());

        payload.append("A");
        assertFalse(payload.isEmpty());
    }

    @Test
    public void str() throws Exception {

        payload.setData("1:2:3:4");
        String expected = "007:1:2:3:4:";

        assertEquals(expected,payload.str());
    }

    @Test
    public void clear() throws Exception {

        payload.setData("1111");
        payload.clear();

        assertEquals("",payload.getData());
    }

    @Test
    public void setData() throws Exception {

        String expected = "1:2:3:4";

        payload.setData(expected);

        assertEquals(expected,payload.getData());
    }

    @Test
    public void toCharArray() throws Exception {

        String msg = "1:2:3:4";

        payload.setData(msg);

        char[] expected = new char[payload.length()];

        expected = payload.str().toCharArray();

        assertArrayEquals(expected,payload.toCharArray());
    }

    @Test
    public void getData() throws Exception {

    }
}