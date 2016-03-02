package fourasians.arduinocontroller;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Button forward = (Button)findViewById(R.id.button);
        Button back = (Button)findViewById(R.id.button2);
        Button left = (Button)findViewById(R.id.button3);
        Button right = (Button)findViewById(R.id.button4);

        //Stop the robot

        forward.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //Move forward();
            }
        });

        back.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //Move forward();
            }
        });
        left.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //Move backwards();
            }
        });

        right.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //Move forward();
            }
        });




    }

}
