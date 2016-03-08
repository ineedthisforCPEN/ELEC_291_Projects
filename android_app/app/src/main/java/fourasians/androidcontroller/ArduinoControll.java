package fourasians.androidcontroller;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;

import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.os.AsyncTask;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class ArduinoControll extends AppCompatActivity {
    private static final int SUCCESS_CONNECT = 0;
    private static final int MESSAGE_READ = 1;
    public int on_off = 3;

    Button btnMov, btnStop, bLeft, bRight, bFunc1, bFunc2, bFunc3;
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket;
    private boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case SUCCESS_CONNECT:
                    //btSocket = (BluetoothSocket)msg.obj;

                    Toast.makeText(getApplicationContext(), "Connected", Toast.LENGTH_SHORT).show();
                    //ConnectedThread connect = new ConnectedThread((BluetoothSocket)msg.obj);
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_arduino_controll); //view of the ledControl layout
        ConnectThread connect = new ConnectThread(MainActivity.device);
        connect.start();

        btnMov = (Button) findViewById(R.id.button2);
        btnStop = (Button) findViewById(R.id.button3);
        bLeft = (Button) findViewById(R.id.button);
        bRight = (Button) findViewById(R.id.button4);
        bFunc1 = (Button) findViewById(R.id.button5);
        bFunc2 = (Button) findViewById(R.id.button6);
        bFunc3 = (Button) findViewById(R.id.button7);

        btnMov.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                turnOnLed();
            }
        });

        btnStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               turnOffLed();
            }
        });

        bLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                moveLeft();
            }
        });

        bRight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                moveRight();
            }
        });

        bFunc1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                func1();
            }
        });

        bFunc2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                func2();
            }
        });

        bFunc3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                func3();
            }
        });


    }

    private void msg(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    private class ConnectThread extends Thread {
        public final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {
            // Use a temporary object that is later assigned to mmSocket,
            // because mmSocket is final
            BluetoothSocket tmp = null;
            mmDevice = device;

            // Get a BluetoothSocket to connect with the given BluetoothDevice
            try {
                // MY_UUID is the app's UUID string, also used by the server code
                tmp = device.createRfcommSocketToServiceRecord(myUUID);
            } catch (IOException e) {
                finish();
            }
            mmSocket = tmp;
        }

        public void run() {
            // Cancel discovery because it will slow down the connection
            //mBluetoothAdapter.cancelDiscovery();

            try {
                // Connect the device through the socket. This will block
                // until it succeeds or throws an exception
                mmSocket.connect();
            } catch (IOException connectException) {
                try {
                    mmSocket.close();
                } catch (IOException closeException) {
                }
                return;
            }
            btSocket = mmSocket;
            mHandler.obtainMessage(SUCCESS_CONNECT, mmSocket).sendToTarget();
        }

        /**
         * Will cancel an in-progress connection, and close the socket
         */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
            }
        }
    }


    public class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final

            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {

            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer;  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    buffer = new byte[1024];
                    bytes = mmInStream.read(buffer);
                    // Send the obtained bytes to the UI activity
                    mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                            .sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(byte[] bytes) {
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) {
            }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
            }
        }
    }


    private void turnOffLed() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("F".toString().getBytes());
            } catch (IOException e) {
                msg("Error");
                finish();
            }
        }
        else
            msg("ERROR");
    }


    private void moveLeft() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("L".toString().getBytes());
            } catch (IOException e) {
                msg("Error");
                finish();
            }
        }
        else
            msg("ERROR");
    }


    private void moveRight() {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write("R".toString().getBytes());
            } catch (IOException e) {
                msg("Error");
                finish();
            }
        }
        else
            msg("ERROR");
    }
    private void turnOnLed()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("O".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
                finish();
            }

        }
        else
            msg("ERROR");
    }

    private void func1()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("X".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
                finish();
            }

        }
        else
            msg("ERROR");
    }

    private void func2()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("Y".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
                finish();
            }

        }
        else
            msg("ERROR");
    }

    private void func3()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("Z".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
                finish();
            }

        }
        else
            msg("ERROR");
    }


}

