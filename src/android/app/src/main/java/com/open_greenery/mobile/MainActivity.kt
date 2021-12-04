package com.open_greenery.mobile

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.Fragment
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.open_greenery.mobile.irrigation.IrrigationSettingsFragment
import com.open_greenery.mobile.relay.RelaySettingsFragment


class MainActivity : AppCompatActivity() {
    private val DEVICE_HOST: String = "192.168.1.53"
    private val LIGHT_PORT: Int = 8090
    private val VENTILATION_PORT: Int = 8091
    private val IRRIGATION_PORT: Int = 8092

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        title = "OpenGreenery"

        val lightFragment = RelaySettingsFragment.newInstance(DEVICE_HOST, LIGHT_PORT)
        val ventilationFragment = RelaySettingsFragment.newInstance(DEVICE_HOST, VENTILATION_PORT)
        val irrigationFragment = IrrigationSettingsFragment.newInstance(DEVICE_HOST, IRRIGATION_PORT)

        setCurrentFragment(lightFragment)

        val bottomNavigationView: BottomNavigationView = findViewById(R.id.bottom_nav_view)
        bottomNavigationView.setOnItemSelectedListener {
            when(it.itemId) {
                R.id.light -> setCurrentFragment(lightFragment)
                R.id.ventilation -> setCurrentFragment(ventilationFragment)
                R.id.irrigation -> setCurrentFragment(irrigationFragment)
            }
            true
        }
    }

    private fun setCurrentFragment(fragment: Fragment) =
        supportFragmentManager.beginTransaction().apply {
            replace(R.id.fragment_layout, fragment)
            commit()
        }
}
