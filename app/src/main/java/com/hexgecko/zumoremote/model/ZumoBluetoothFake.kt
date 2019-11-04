package com.hexgecko.zumoremote.model

import android.util.Log
import androidx.lifecycle.MutableLiveData

class ZumoBluetoothFake: ZumoBluetooth {
    companion object {
        const val TAG = "ZumoBluetoothFake"
    }

    override val speed = MutableLiveData<Pair<Int,Int>>().apply { value = Pair(0,0) }

    override val error = MutableLiveData<String>()

    override fun onSetSpeed(left: Int, right: Int) {
        Log.d(TAG, "Speed: left: $left, right: $right")

        speed.value = Pair(left, right)
    }

    override fun onStop() {
        error.value = "Test error"
        speed.value = Pair(0,0)
    }
}
