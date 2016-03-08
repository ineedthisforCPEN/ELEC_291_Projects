package fourasians.androidcontroller;

import android.app.Application;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

import android.widget.Button;
import android.widget.ListView;

import java.io.IOException;
import java.util.Set;
import java.util.ArrayList;
import java.util.UUID;

import android.widget.Toast;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.content.Intent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

public class MainActivity extends AppCompatActivity {


    private static final int SUCCESS_CONNECT = 0;
    Button btnPaired;
    ListView devicelist;
    static BluetoothDevice device;
    private BluetoothAdapter myBluetooth = null;
    private Set<BluetoothDevice> pairedDevices;
    public static final String EXTRA_ADDRESS = "string";
    public static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnPaired = (Button)findViewById(R.id.bConnectNew);
        devicelist = (ListView)findViewById(R.id.listView);
        //get bluetooth adapter
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        if(myBluetooth == null)
        {
            //Show a mensag. that thedevice has no bluetooth adapter
            Toast.makeText(getApplicationContext(), "Bluetooth Device Not Available", Toast.LENGTH_LONG).show();
            //finish apk
            finish();
        }
        //enable bluetooth
        else
        {
            if (!myBluetooth.isEnabled()) {


                //Ask to the user turn the bluetooth on
                Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(turnBTon,1);
            }
        }

        btnPaired.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pairedDevicesList(); //method that will be called
            }
        });

    }

    private void pairedDevicesList()
    {
        //querying paired devices
        pairedDevices = myBluetooth.getBondedDevices();
        ArrayList<String> list = new ArrayList<String>();

        if (pairedDevices.size()>0)
        {
            for(BluetoothDevice dev : pairedDevices)
            {
                list.add(dev.getName() + "\n" + dev.getAddress()); //Get the device's name and the addres
                device = dev;
            }
        }
        else
        {
            Toast.makeText(getApplicationContext(), "No Paired Bluetooth Devices Found.", Toast.LENGTH_LONG).show();
        }

        final ArrayAdapter adapter = new ArrayAdapter(this,android.R.layout.simple_list_item_1, list);
        devicelist.setAdapter(adapter);
        devicelist.setOnItemClickListener(myListClickListener); //Method called when the device from the list is clicked

    }

    private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener()
    {
        public void onItemClick (AdapterView av, View v, int arg2, long arg3)
        {
            // Get the device MAC address, the last 17 chars in the View
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);
            //ConnectThread connect = new ConnectThread(selectDevice);
            //connect.start();
            // Make an intent to start next activity.
            Intent i = new Intent(MainActivity.this, ArduinoControll.class);
            //Change the activity.
            i.putExtra(EXTRA_ADDRESS, address); //this will be received at ledControl (class) Activity
            startActivity(i);
        }
    };
}
