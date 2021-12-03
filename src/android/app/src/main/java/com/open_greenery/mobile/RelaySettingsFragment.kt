package com.open_greenery.mobile

import android.app.TimePickerDialog
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.TimePicker
import androidx.core.view.isVisible
import com.google.android.material.button.MaterialButton
import com.google.android.material.switchmaterial.SwitchMaterial
import java.time.LocalTime

// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_RPC_HOST = "rpcHost"
private const val ARG_RPC_PORT = "rpcPort"

private fun Int.toTimeString() = this.toString().padStart(2, '0')

/**
 * A simple [Fragment] subclass.
 * Use the [RelaySettingsFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class RelaySettingsFragment : Fragment(R.layout.fragment_relay_settings) {
    private var relayRpcClient: RelayRpcClient? = null

    private class RelayTimePicker
        (private val editText: EditText, private val timeSetter: (LocalTime) -> Unit)
        : TimePickerDialog.OnTimeSetListener {
        override fun onTimeSet(view: TimePicker?, hourOfDay: Int, minute: Int) {
            val time = "${hourOfDay.toTimeString()}:${minute.toTimeString()}"
            editText.setText(time)
            timeSetter(LocalTime.of(hourOfDay, minute))
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            relayRpcClient = RelayRpcClient(it.getString(ARG_RPC_HOST)!!, it.getInt(ARG_RPC_PORT))
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        val view = inflater.inflate(R.layout.fragment_relay_settings, container, false)

        // Mode toggle
        val relayModeAutoButton: MaterialButton = view.findViewById(R.id.relay_mode_auto_button)
        val relayModeManualButton: MaterialButton = view.findViewById(R.id.relay_mode_manual_button)
        val dayConfigurationLayout: LinearLayout = view.findViewById(R.id.day_configuration_layout)
        val selectModeToggle = { mode: Mode ->
            val autoButtonSelected = (mode == Mode.AUTO)
            relayModeAutoButton.isChecked = autoButtonSelected
            relayModeManualButton.isChecked = !autoButtonSelected
            dayConfigurationLayout.isVisible = autoButtonSelected
        }
        relayModeAutoButton.setOnClickListener {
            selectModeToggle(Mode.AUTO)
            relayRpcClient?.mode = Mode.AUTO
        }
        relayModeManualButton.setOnClickListener {
            selectModeToggle(Mode.MANUAL)
            relayRpcClient?.mode = Mode.MANUAL
        }

        // Relay state switch
        val relayStateSwitch: SwitchMaterial = view.findViewById(R.id.relay_state_switch)
        val (mode, relayEnabled, config) = relayRpcClient!!.service_status
        selectModeToggle(mode)
        relayStateSwitch.isChecked = relayEnabled
        relayStateSwitch.setOnCheckedChangeListener {
                _, isChecked ->
            if (isChecked) {
                relayRpcClient?.enable()
            }
            else {
                relayRpcClient?.disable()
            }
        }

        // Relay day config text fields
        val relayDayStartEdit: EditText = view.findViewById(R.id.relay_day_start_edit)
        val relayDayEndEdit: EditText = view.findViewById(R.id.relay_day_end_edit)
        relayDayStartEdit.setText(config.dayStart.toString())
        relayDayEndEdit.setText(config.dayEnd.toString())
        // Time pickers
        relayDayStartEdit.setOnClickListener {
            val picker = TimePickerDialog(
                view.context,
                RelayTimePicker(
                    relayDayStartEdit,
                    relayRpcClient!!::updateDayStart
                ),
                7,
                0,
                true)
            picker.show()
        }
        relayDayEndEdit.setOnClickListener {
            val picker = TimePickerDialog(
                view.context,
                RelayTimePicker(
                    relayDayEndEdit,
                    relayRpcClient!!::updateDayEnd
                ),
                19,
                0,
                true)
            picker.show()
        }

        return view
    }

    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param rpcHost RPC host URL
         * @param rpcPort RPC port
         * @return A new instance of fragment RelaySettingsFragment.
         */
        @JvmStatic
        fun newInstance(rpcHost: String, rpcPort: Int) =
            RelaySettingsFragment().apply {
                arguments = Bundle().apply {
                    putString(ARG_RPC_HOST, rpcHost)
                    putInt(ARG_RPC_PORT, rpcPort)
                }
            }
    }
}