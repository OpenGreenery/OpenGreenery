package com.open_greenery.mobile

import android.app.TimePickerDialog
import android.os.Bundle
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TimePicker
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.isVisible
import androidx.preference.PreferenceFragmentCompat
import com.google.android.material.button.MaterialButton
import com.google.android.material.switchmaterial.SwitchMaterial
import java.time.LocalTime

fun Int.toTimeString() = this.toString().padStart(2, '0')

class RelaySettingsActivity : AppCompatActivity() {
    class RelayTimePicker
        (private val editText: EditText, private val timeSetter: (LocalTime) -> Unit)
        : TimePickerDialog.OnTimeSetListener {
        override fun onTimeSet(view: TimePicker?, hourOfDay: Int, minute: Int) {
            val time = "${hourOfDay.toTimeString()}:${minute.toTimeString()}"
            editText.setText(time)
            timeSetter(LocalTime.of(hourOfDay, minute))
        }
    }

    private lateinit var relay: RelayRpcClient
    private lateinit var relayStateSwitch: SwitchMaterial
    private lateinit var relayModeAutoButton: MaterialButton
    private lateinit var relayModeManualButton: MaterialButton
    private lateinit var dayConfigurationLayout: LinearLayout
    private lateinit var relayDayStartEdit: EditText
    private lateinit var relayDayEndEdit: EditText
    private lateinit var dayStartPickerDialogListener: RelayTimePicker
    private lateinit var dayEndPickerDialogListener: RelayTimePicker


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
        val (mode, relayEnabled, config) = relay.service_status
        selectModeToggle(mode)
        relayStateSwitch.isChecked = relayEnabled
        relayDayStartEdit.setText(config.dayStart.toString())
        relayDayEndEdit.setText(config.dayEnd.toString())
        dayStartPickerDialogListener = RelayTimePicker(relayDayStartEdit, relay::updateDayStart)
        dayEndPickerDialogListener = RelayTimePicker(relayDayEndEdit, relay::updateDayEnd)

        relayDayStartEdit.setOnClickListener {
            val picker = TimePickerDialog(this, dayStartPickerDialogListener, 7, 0, true)
            picker.show()
        }
        relayDayEndEdit.setOnClickListener {
            val picker = TimePickerDialog(this, dayEndPickerDialogListener, 19, 0, true)
            picker.show()
        }

        relayModeAutoButton.setOnClickListener {
            selectModeToggle(Mode.AUTO)
            relay.mode = Mode.AUTO
        }
        relayModeManualButton.setOnClickListener {
            selectModeToggle(Mode.MANUAL)
            relay.mode = Mode.MANUAL
        }

        relayStateSwitch.setOnCheckedChangeListener {
                _, isChecked ->
            if (isChecked) {
                relay.enable()
            }
            else {
                relay.disable()
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
