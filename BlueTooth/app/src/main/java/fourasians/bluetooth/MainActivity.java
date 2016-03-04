package fourasians.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class MainActivity extends AppCompatActivity {

    ArrayAdapter<String> listAdapter;
    Button connectNew;
    ListView listView;
    BluetoothAdapter btAdapter;
    Set<BluetoothDevice> devicesArray;
    ArrayList<String> pairedDevices;
    IntentFilter filter;
    BroadcastReceiver receiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
        if(btAdapter == null){
            Toast.makeText(getApplicationContext(), "Bluetooth not supported on this device!", Toast.LENGTH_SHORT).show();
            finish();
        }

        else {
            if(!btAdapter.isEnabled())
            {
                Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(intent, 1);
            }
            
            getPairedDevices();
        }
    }

    private void getPairedDevices() {
        devicesArray = btAdapter.getBondedDevices();
        if(devicesArray.size() > 0) {
            for(BluetoothDevice device : devicesArray)
            {
                pairedDevices.add(device.getName());
            }
        }
    }

    private void init() {
        connectNew = (Button)findViewById(R.id.bConnectNew);
        listView = (ListView)findViewById(R.id.listView);
        listAdapter = new ArrayAdapter<String> (this,android.R.layout.simple_list_item_1,0);
        listView.setAdapter(listAdapter);
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        pairedDevices = new ArrayList<String>();
        filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();

                if(BluetoothDevice.ACTION_FOUND.equals(action))
                {
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    listAdapter.add(device.getName() + "/n" + device.getAddress());
                }

                else if(BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action))
                {

                }

                else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action))
                {

                }
                else if(BluetoothAdapter.ACTION_STATE_CHANGED.equals(action))
                {

                }
            }
        };

        registerReceiver(receiver, filter);
        IntentFilter filtertwo = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        registerReceiver(receiver, filtertwo);
        IntentFilter filterthree = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(receiver, filterthree);
        IntentFilter filterfour = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(receiver, filterfour);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(resultCode == RESULT_CANCELED)
        {
            Toast.makeText(getApplicationContext(), "Enable bluetooth to continue!", Toast.LENGTH_SHORT).show();
            finish();
        }
    }
}
