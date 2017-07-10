package com.example.jefferson.goodstracker.Communication;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;
import com.example.jefferson.goodstracker.Domain.Tracker;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by Jefferson on 10/07/2017.
 */
public class CommunicationTest {

    Communic            communic;
    ObservableAnswerCmd observable;
    static Tracker[]    trackers;
    @BeforeClass
    public static void onceExecutedBeforeAll() {

        Communication.create(TYPE_COMMUNICATION.AMQP);

        trackers = new Tracker[]{ new Tracker(0),
                                            new Tracker(1),
                                            new Tracker(2),
                                            new Tracker(3),
                                            new Tracker(4)};
    }

    @Before
    public void executedBeforeEach() {

        observable  = (ObservableAnswerCmd)Communication.getInstance();
        communic    = Communication.getInstance();
    }

    @Test
    public void init() throws Exception {

    }

    @Test
    public void deInit() throws Exception {

    }

    @Test
    public void create() throws Exception {

    }

    @Test
    public void isAnyTxCmd() throws Exception {

    }

    @Test
    public void isAnyAns() throws Exception {

    }

    @Test
    public void isAnyQueueCmd() throws Exception {

    }

    @Test
    public void addAns() throws Exception {

    }

    @Test
    public void removeAns() throws Exception {

    }

    @Test
    public void removeTxCmd() throws Exception {

    }

    @Test
    public void addTxCmd() throws Exception {

    }

    @Test
    public void removeCmd() throws Exception {

    }

    @Test
    public void addCmd() throws Exception {

    }

    @Test
    public void searchCmdOfAnswer() throws Exception {

    }

    @Test
    public void getArrayOfUnit() throws Exception {

    }

    @Test
    public void getArrayOfCmd() throws Exception {

    }

    @Test
    public void acceptAnswer() throws Exception {

    }

    @Test
    public void getNextCmd() throws Exception {

    }

    @Test
    public void initThread() throws Exception {

    }

    @Test
    public void registerObserver() throws Exception {

    }

    @Test
    public void removeObserver() throws Exception {

    }

    @Test
    public void notifyAllObservers() throws Exception {

    }

    @Test
    public void notifyObserver() throws Exception {

         final AnsCmd[] list = new AnsCmd[trackers.length];

        int i = 0;
        for(Tracker t:trackers){

            t.createCMD(Operation.RD, RESOURCE_TYPE.TLM, new EventReceiveAnswer() {
                int y = 0;
                @Override
                public void onReceiveAnswer(AnsCmd ans) {

                    list[y++] = ans;
                }
            });

            AnsCmd  ans     = new AnsCmd();
            Header header   = new Header();

            header.setAddress(i++);
            header.setDest(0);
            header.setOperation(Operation.AN);
            header.setResource(RESOURCE_TYPE.TLM);
            ans.setHeader(header);
            ans.setTelemetria(new DataTelemetria());

            observable.notifyObserver(ans);
        }

        for(AnsCmd a:list){

            assertEquals(RESOURCE_TYPE.TLM,a.getResource());
        }
    }

    @Test
    public void getCommunic() throws Exception {

        assertNotNull(Communication.getInstance());
    }
}