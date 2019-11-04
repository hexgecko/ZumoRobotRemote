package com.hexgecko.zumoremote

import android.annotation.SuppressLint
import android.app.Application
import com.hexgecko.zumoremote.model.ZumoBluetooth
import com.hexgecko.zumoremote.model.ZumoBluetoothProd
import org.koin.core.context.startKoin
import org.koin.dsl.module

@SuppressLint("Registered")
class ZumoRemote: Application() {
    override fun onCreate() {
        super.onCreate()

        startKoin {
            modules(module {
                single<ZumoBluetooth> { ZumoBluetoothProd(this@ZumoRemote) }
            })
        }
    }
}
