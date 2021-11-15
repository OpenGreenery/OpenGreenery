package com.open_greenery.mobile

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.content.Intent
import android.widget.Button


class MainActivity : AppCompatActivity() {
    private val DEVICE_HOST: String = "192.168.1.53"
    private val LIGHT_PORT: Int = 8090
    private val VENTILATION_PORT: Int = 8091

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        title = "OpenGreenery"
        findViewById<Button>(R.id.light_menu_button).setOnClickListener {
            startRelaySettingsActivity(
                "Light Settings",
                DEVICE_HOST, LIGHT_PORT
            )
        }
        findViewById<Button>(R.id.ventilation_menu_button).setOnClickListener {
            startRelaySettingsActivity(
                "Ventilation Settings",
                DEVICE_HOST, VENTILATION_PORT
            )
        }
    }

    private fun startRelaySettingsActivity(title: String, host: String, port: Int) {
        val intent = Intent(this@MainActivity, RelaySettingsActivity::class.java)
        intent.putExtra("title", title)
        intent.putExtra("host", host)
        intent.putExtra("port", port)
        startActivity(intent)
    }
}
