package com.example.jefferson.goodstracker.Domain;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by Jefferson on 10/07/2017.
 */
public class TolTest {

    Tol tol;

    @BeforeClass
    public static void onceExecutedBeforeAll() {

    }

    @Before
    public void executedBeforeEach() {

        tol = new Tol(100,200);
    }

    @Test
    public void getMin() throws Exception {

    }

    @Test
    public void setMin() throws Exception {

    }

    @Test
    public void getMax() throws Exception {

    }

    @Test
    public void setMax() throws Exception {

    }

    @Test
    public void OK() throws Exception {

        assertTrue(tol.OK(150));
        assertTrue(tol.OK(200));
        assertTrue(tol.OK(100));
        assertFalse(tol.OK(99));
        assertFalse(tol.OK(251));
    }
}