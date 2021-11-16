package com.open_greenery.mobile

import android.os.Bundle
import android.widget.EditText
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.preference.PreferenceFragmentCompat
import com.google.android.material.button.MaterialButton
import com.google.android.material.switchmaterial.SwitchMaterial

class RelaySettingsActivity : AppCompatActivity() {
    private lateinit var relayStateSwitch: SwitchMaterial
    private lateinit var relayModeAutoButton: MaterialButton
    private lateinit var relayModeManualButton: MaterialButton
    private lateinit var dayConfigurationLayout: LinearLayout
    private lateinit var relayDayStartEdit: EditText
    private lateinit var relayDayEndEdit: EditText

    var relay: RelayRpcClient? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.relay_settings_activity)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        relayStateSwitch = findViewById(R.id.relay_state_switch)
        relayModeAutoButton = findViewById(R.id.relay_mode_auto_button)
        relayModeManualButton = findViewById(R.id.relay_mode_manual_button)
        dayConfigurationLayout = findViewById(R.id.day_configuration_layout)
        relayDayStartEdit = findViewById(R.id.relay_day_start_edit)
        relayDayEndEdit = findViewById(R.id.relay_day_end_edit)

        val extras: Bundle = intent.extras!!
        title = extras.getString("title")
        relay = RelayRpcClient(
            address = extras.getString("host")!!,
            port = extras.getInt("port")
        )
        val (mode, relayEnabled, config) = relay!!.service_status
        selectModeToggle(mode)
        relayStateSwitch.isChecked = relayEnabled
        relayDayStartEdit.setText(config.dayStart.toString())
        relayDayEndEdit.setText(config.dayEnd.toString())

        relayModeAutoButton.setOnClickListener {
            selectModeToggle(Mode.AUTO)
        }
        relayModeManualButton.setOnClickListener {
            selectModeToggle(Mode.MANUAL)
        }

        relayStateSwitch.setOnCheckedChangeListener {
                _, isChecked ->
            if (isChecked) {
                relay!!.enable()
            }
            else {
                relay!!.disable()
            }
        }
    }

    class SettingsFragment : PreferenceFragmentCompat() {
        override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
            setPreferencesFromResource(R.xml.root_preferences, rootKey)
        }
    }

    private fun selectModeToggle(m: Mode) {
        val autoButtonSelected = (m == Mode.AUTO)
        relayModeAutoButton.isChecked = autoButtonSelected
        relayModeManualButton.isChecked = !autoButtonSelected
        dayConfigurationLayout.isVisible = autoButtonSelected
    }

    override fun onSupportNavigateUp(): Boolean {
        onBackPressed()
        return true
    }
}
