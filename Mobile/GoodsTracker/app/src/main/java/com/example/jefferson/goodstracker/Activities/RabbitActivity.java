package com.example.jefferson.goodstracker.Activities;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.example.jefferson.goodstracker.Communication.RabbitMQ;
import com.example.jefferson.goodstracker.R;

import java.text.SimpleDateFormat;
import java.util.Date;

public class RabbitActivity extends AppCompatActivity {

    RabbitMQ rabbitMQ = new RabbitMQ();

    private TextView mTextMessage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_rabbit);

        rabbitMQ.open();
        rabbitMQ.createConnThread();
        rabbitMQ.createPublishThread();
        rabbitMQ.createSubscribeThread(incomingMessageHandler);

        mTextMessage = (TextView) findViewById(R.id.message);
        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
    }

    public void onClick_btConnect(View view){

        try {

            rabbitMQ.connect();

        } catch (InterruptedException e) {

            e.printStackTrace();
        }
    };

    public void onClick_btPublish(View view){

        EditText et = (EditText) findViewById(R.id.text);
        rabbitMQ.putLast(et.getText().toString());
        et.setText("");
    };

    final Handler incomingMessageHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {

        String message = msg.getData().getString("msg");
        TextView tv = (TextView) findViewById(R.id.textView);
        Date now = new Date();
        SimpleDateFormat ft = new SimpleDateFormat ("hh:mm:ss");
        tv.append(ft.format(now) + ' ' + message + '\n');
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();

        rabbitMQ.close();
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
