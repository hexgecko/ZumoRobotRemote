package com.hexgecko.zumoremote.model

import android.app.Application
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothSocket

import androidx.lifecycle.MutableLiveData
import org.json.JSONObject
import java.io.*
import java.lang.Exception
import java.util.*

class ZumoBluetoothProd(val application: Application): ZumoBluetooth {
    companion object {
        const val DEVICE_NAME = "SumoRobot"
        private val SPP_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    }

    private val adapter = BluetoothAdapter.getDefaultAdapter()
    private var socket: BluetoothSocket? = null

    private var speedSetPoint = Pair(0,0)
    private var speedLastSetPoint = Pair(0,0)

    override val speed = MutableLiveData<Pair<Int,Int>>()

    override val error = MutableLiveData<String>()

    override fun onSetSpeed(left: Int, right: Int) {
        speedSetPoint = Pair(left, right)
    }

    override fun onStop() {
        speedSetPoint = Pair(0, 0)
    }

    init {
        if(adapter == null) {
            error.value = "Bluetooth adapter is not available!"
        } else if(!adapter.isEnabled) {
            error.value = "Bluetooth is disabled, check setting!"
        } else {
            // loop through the connected devices to find our controller.
            val device = adapter.bondedDevices.find { it.name == DEVICE_NAME }
            adapter.cancelDiscovery()

            if(device == null) {
                error.value = "Bluetooth target not found!"
            } else {
                // Try to create a new socket
                try {
                    socket = device.createRfcommSocketToServiceRecord(SPP_UUID)
                } catch(e: IOException) {
                    error.value = e.message
                }

                ConnectThread().start()
            }
        }
    }

    inner class ConnectThread: Thread() {
        override fun run() {
            // Try to connect to the socket
            if(socket != null) {
                var i=0
                var connected = false
                while(!connected) {
                    try {
                        socket?.connect()
                    } catch (e: IOException) {
                        if(i++ < 3) {
                            sleep(100)
                            continue
                        }
                        error.postValue(e.toString())
                    }
                    connected = true
                }

                sleep(100)
                val writer = BufferedWriter(OutputStreamWriter(socket!!.outputStream, "ASCII"))
                val reader = BufferedReader(InputStreamReader(socket!!.inputStream, "ASCII"))
                writer.write("{\"cmd\":\"status\"}\n")
                writer.flush()
                sleep(100)

                i=0
                while(true) {
                    try {
                        writer.write("{\"cmd\":\"status\"}\n")
                        writer.flush()

                        sleep(30)

                        val json = JSONObject(reader.readLine())
                        val left = json.getInt("left")
                        val right = json.getInt("right")

                        speed.postValue(Pair(left, right))

                        if(speedSetPoint != speedLastSetPoint) {
                            writer.write("{\"cmd\":\"motor\",\"left\":${speedSetPoint.first},\"right\":${speedSetPoint.second}}")
                            writer.flush()

                            speedLastSetPoint = speedSetPoint
                        }

                        sleep(30)

                    } catch (e: Exception) {
                        if(i++ < 3) {
                            sleep(1000)
                            continue
                        }
                        error.postValue(e.toString())
                    }
                }
            } else {
                error.postValue("Bluetooth device is not connected!")
            }
        }
    }
}