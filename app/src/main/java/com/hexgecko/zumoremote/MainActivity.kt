package com.hexgecko.zumoremote

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import android.widget.Toast
import androidx.lifecycle.LiveData
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import com.hexgecko.zumoremote.viewmodel.MainActivityViewModel
import com.jmedeisis.bugstick.Joystick
import com.jmedeisis.bugstick.JoystickListener
import kotlin.math.cos
import kotlin.math.sin

class MainActivity : AppCompatActivity() {
    companion object {
        const val MAX_SPEED = 400
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val viewModel = ViewModelProviders.of(this).get(MainActivityViewModel::class.java)

        bindJoystick(findViewById(R.id.joystick), viewModel::onDragJoystick, viewModel::onReleaseJoystick)
        bindError(viewModel.error)
        bindSpeed(findViewById(R.id.left_speed), findViewById(R.id.right_speed), viewModel.speed)
    }

    private fun bindJoystick(joystick: Joystick, onDrag: (left: Int, right: Int) -> Unit, onRelease: () -> Unit) {
        joystick.setJoystickListener(object: JoystickListener {
            override fun onDrag(degrees: Float, offset: Float) {
                val up = sin(degrees.toDouble() * Math.PI / 180.0) * offset
                val side = cos(degrees.toDouble() * Math.PI / 180.0) * offset

                val left = ((up + side) * MAX_SPEED).toInt()
                val right = ((up - side) * MAX_SPEED).toInt()
                onDrag(left, right)
            }

            override fun onDown() { }

            override fun onUp() { onRelease() }
        })
    }

    private fun bindError(error: LiveData<String>) {
        error.observe(this, Observer {
            Toast.makeText(applicationContext, it, Toast.LENGTH_LONG).show()
        })
    }

    private fun bindSpeed(leftText: TextView, rightText: TextView, speed: LiveData<Pair<Int,Int>>) {
        speed.observe(this, Observer {
            leftText.text = resources.getString(R.string.text_left_speed).format(it.first)
            rightText.text = resources.getString(R.string.text_right_speed).format(it.second)
        })
    }
}
