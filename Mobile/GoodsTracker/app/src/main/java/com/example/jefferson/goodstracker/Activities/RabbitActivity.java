package com.example.jefferson.goodstracker.Activities;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import com.example.jefferson.goodstracker.Communication.Communication;
import com.example.jefferson.goodstracker.Communication.TYPE_COMMUNICATION;
import com.example.jefferson.goodstracker.Domain.Tracker;
import com.example.jefferson.goodstracker.R;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class RabbitActivity extends AppCompatActivity {

    private TextView        mTextMessage;
    BottomNavigationView    navigation;
    TextView                tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_rabbit);

        Communication.create(TYPE_COMMUNICATION.AMQP);

        mTextMessage    = (TextView)                findViewById(R.id.message);
        navigation      = (BottomNavigationView)    findViewById(R.id.navigation);
        tv              = (TextView)                findViewById(R.id.textView);

        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
    }

    public void onClick_btConnect(View view){

        try {

            Tracker[]   trackers = new Tracker[5];

            for(int i=0;i<trackers.length;i++){

                trackers[i] = new Tracker(i+1);
                trackers[i].requestBehavior();
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    };

    public void onClick_btPublish(View view){

//        EditText et = (EditText) findViewById(R.id.text);
//        rabbitMQ.putLast(et.getText().toString());
//        et.setText("");
    };

    final Handler incomingMessageHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {

            String message  = msg.getData().getString("msg");
            Date now        = new Date();
            SimpleDateFormat ft = new SimpleDateFormat ("hh:mm:ss");
            tv.append(ft.format(now) + ' ' + message + '\n');
        }
    };

    @Override
    protected void onDestroy() {

        super.onDestroy();

        Communication.create(TYPE_COMMUNICATION.NONE);
    }

    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
        = new BottomNavigationView.OnNavigationItemSelectedListener() {

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {

        switch (item.getItemId()) {

            case R.id.navigation_home:
                mTextMessage.setText(R.string.title_home);
                return true;
            case R.id.navigation_dashboard:
                mTextMessage.setText(R.string.title_dashboard);
                return true;
            case R.id.navigation_notifications:
                mTextMessage.setText(R.string.title_notifications);
                return true;
        }

        return false;
        }

    };
}
