package com.open_greenery.mobile

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.preference.PreferenceFragmentCompat
import com.google.android.material.button.MaterialButton
import com.google.android.material.button.MaterialButtonToggleGroup
import com.google.android.material.switchmaterial.SwitchMaterial

class RelaySettingsActivity : AppCompatActivity() {

    var relay: RelayRpcClient? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.relay_settings_activity)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        val extras: Bundle = intent.extras!!
        title = extras.getString("title")
        relay = RelayRpcClient(
            address = extras.getString("host")!!,
            port = extras.getInt("port")
        )

        findViewById<MaterialButton>(R.id.relay_mode_auto_button).setOnClickListener {
            selectModeToggle(Mode.AUTO)
        }
        findViewById<MaterialButton>(R.id.relay_mode_manual_button).setOnClickListener {
            selectModeToggle(Mode.MANUAL)
        }

        findViewById<SwitchMaterial>(R.id.relay_state_switch).setOnCheckedChangeListener {
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

    private enum class Mode {
        MANUAL, AUTO
    }

    private fun selectModeToggle(m: Mode) {
        if (m == Mode.AUTO) {
            findViewById<MaterialButton>(R.id.relay_mode_auto_button).isChecked = true
            findViewById<MaterialButton>(R.id.relay_mode_manual_button).isChecked = false
            setTimeInputFieldVisible(true)
        }
        else {
            findViewById<MaterialButton>(R.id.relay_mode_auto_button).isChecked = false
            findViewById<MaterialButton>(R.id.relay_mode_manual_button).isChecked = true
            setTimeInputFieldVisible(false)
        }
    }

    private fun setTimeInputFieldVisible(visible: Boolean) {
        findViewById<MaterialButtonToggleGroup>(R.id.day_configuration_layout).isVisible = visible
    }

    override fun onSupportNavigateUp(): Boolean {
        onBackPressed()
        return true
    }
}
