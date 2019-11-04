package com.hexgecko.zumoremote.model

import androidx.lifecycle.LiveData

interface ZumoBluetooth {
    val speed: LiveData<Pair<Int, Int>>
    val error: LiveData<String>
    fun onSetSpeed(left: Int, right: Int)
    fun onStop()
}