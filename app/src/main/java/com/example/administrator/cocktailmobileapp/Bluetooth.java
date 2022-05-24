package com.example.administrator.cocktailmobileapp;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class Bluetooth {
    private static final String TAG = "Bluetooth";

    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");    //UUID
    private static final String MAC_ADDRESS = "98:DA:60:01:9E:19";  //아두이노 맥주소

    private static Bluetooth instance;

    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice mBluetoothDevice;
    private BluetoothSocket mBluetoothSocket;
    private OutputStream mOutputStream;
    private InputStream mInputStream;

    private String message;

    public static Bluetooth getInstance() {
        if (instance == null) {
            instance = new Bluetooth();
        }
        return instance;
    }

    // 칵테일 제조를 위한 인터페이스
    public interface BluetoothCall {
        void onMaking();
        void onComplete();
    }

    // 온도를 불러오기 위한 인터페이스
    public interface BluetoothSet{
        void setView(int temp);
    }

    public Bluetooth() {
        try {
            mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            mBluetoothDevice = mBluetoothAdapter.getRemoteDevice(MAC_ADDRESS);
            mBluetoothSocket = mBluetoothDevice.createInsecureRfcommSocketToServiceRecord(MY_UUID);

            mBluetoothSocket.connect();

            mOutputStream = mBluetoothSocket.getOutputStream();
            mInputStream = mBluetoothSocket.getInputStream();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeData(String data) {
        try {
            mOutputStream.write(data.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // 블루투스로 온도를 받아와서 첫 화면에 온도 출력
    public void tempData(final BluetoothSet bluetoothSet){
        final Handler mHandler = new Handler();
         Thread thread = new Thread(new Runnable() {
             @Override
             public void run() {
                 mHandler.post(new Runnable() {
                     @Override
                     public void run() {
                         try {
                             while (true) {
                                 if (mInputStream.available() > 0) {
                                     byte[] buffer = new byte[mInputStream.available()];

                                     try {
                                         mInputStream.read(buffer);
                                     } catch (IOException e) {
                                         e.printStackTrace();
                                     }
                                     message = new String(buffer);
                                     int temp = Integer.parseInt(message);

                                     if(temp>=0 && temp <= 50){
                                         bluetoothSet.setView(temp);
                                         break;
                                     }else{
                                         // 온도가 비 정상적일 때
                                         bluetoothSet.setView(999999);
                                     }
                                     System.out.println("test Meseage:" + message);
                                 }

                             }
                         } catch (IOException e) {
                             e.printStackTrace();
                         }
                     }
                 });
             }
         });
         thread.start();
    }

    public void readData(final Handler handler, final BluetoothCall bluetoothCall) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (true) {
                        /*
                         * 여기서 메이킹 애니매이션의 속도를 바꿀 수 있습니다.
                         * 1000 - 1초
                         */
                        Thread.sleep(1000);

                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                bluetoothCall.onMaking();
                            }
                        });

                        if (mInputStream.available() > 0) {
                            byte[] buffer = new byte[mInputStream.available()];

                            mInputStream.read(buffer);
                            String text = new String(buffer);

                            Log.d(TAG, "size - " + mInputStream.available() + " text - " + text);

                            if (text.equals("9")) {
                                handler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        bluetoothCall.onComplete();
                                    }
                                });

                                break;
                            }
                        }
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });

        thread.start();
    }


}
