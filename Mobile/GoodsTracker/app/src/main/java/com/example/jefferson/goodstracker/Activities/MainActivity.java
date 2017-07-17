package com.example.jefferson.goodstracker.Activities;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;

import com.example.jefferson.goodstracker.Communication.Communication;
import com.example.jefferson.goodstracker.Communication.TYPE_COMMUNICATION;
import com.example.jefferson.goodstracker.R;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        Communication.create(TYPE_COMMUNICATION.AMQP);

        showFragment(new TrackerHome());

        // Toolbar
        Toolbar toolbar         = (Toolbar) findViewById(R.id.toolbar);
        DrawerLayout drawer     = (DrawerLayout) findViewById(R.id.drawer_layout);

        setSupportActionBar(toolbar);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        // Botton menu
        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);

        // Menu Lateral
        NavigationView navigationView   = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {

        int id = item.getItemId();

        switch (id){

            case R.id.nav_trip:     showTripActivit();      break;
            case R.id.nav_camera:                           break;
            case R.id.nav_gallery:                          break;
            case R.id.nav_slideshow:                        break;
            case R.id.nav_manage:                           break;
            case R.id.nav_share:                            break;
            case R.id.nav_send:                             break;
            case R.id.nav_server:   showServerActivity();   break;
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }

    private void showServerActivity() {

        Intent it = new Intent(this,   RabbitActivity.class);
        startActivity(it);
    }

    public void showTripActivit(){

        Intent it = new Intent(this,   TripActivity.class);
        startActivity(it);
    }

    /**
     * Handle bottom menu
     *
     */
    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {

                case R.id.navigation_home:          showFragment(new TrackerHome());
                    return true;
                case R.id.navigation_dashboard:     showFragment(new TrackerDashBoard());
                    return true;
                case R.id.navigation_notifications: showFragment(new TrackerNotification());
                    return true;
            }
            return false;
        }
    };

    protected void showFragment(Fragment fragment) {

        FragmentTransaction t = getSupportFragmentManager().beginTransaction();
        t.replace(R.id.content, fragment);
        t.commit();
    }
}
