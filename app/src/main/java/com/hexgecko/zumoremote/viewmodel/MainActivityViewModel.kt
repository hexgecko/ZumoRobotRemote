package com.hexgecko.zumoremote.viewmodel

import androidx.lifecycle.ViewModel
import com.hexgecko.zumoremote.model.ZumoBluetooth
import org.koin.core.KoinComponent
import org.koin.core.inject

class MainActivityViewModel: ViewModel(), KoinComponent {

    private val zumoBluetooth by inject<ZumoBluetooth>()

    val speed = zumoBluetooth.speed
    val error = zumoBluetooth.error

    fun onDragJoystick(left: Int, right: Int) {
        zumoBluetooth.onSetSpeed(left, right)
    }

    fun onReleaseJoystick() {
        zumoBluetooth.onStop()
    }
}
