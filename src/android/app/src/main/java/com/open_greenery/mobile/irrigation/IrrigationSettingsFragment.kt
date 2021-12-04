package com.open_greenery.mobile.irrigation

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import androidx.core.widget.addTextChangedListener
import com.google.android.material.button.MaterialButton
import com.open_greenery.mobile.R
import com.open_greenery.mobile.relay.RelayRpcClient
import java.time.Duration

private const val ARG_RPC_HOST = "rpcHost"
private const val ARG_RPC_PORT = "rpcPort"

private fun Button.makeActive() {
    this.setBackgroundColor(resources.getColor(R.color.dark_button_background))
    this.setTextColor(resources.getColor(R.color.light_text))
}
private fun Button.makeInactive() {
    this.setBackgroundColor(resources.getColor(R.color.light_button_background))
    this.setTextColor(resources.getColor(R.color.dark_text))
}

/**
 * A simple [Fragment] subclass.
 * Use the [IrrigationSettingsFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class IrrigationSettingsFragment : Fragment(R.layout.fragment_irrigation_settings) {
    private var relayRpcClient: RelayRpcClient? = null
    private val areaConfigurationMux = AreaConfigurationMux()

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
        val view = inflater.inflate(
            R.layout.fragment_irrigation_settings,
            container,
            false
        )

        val irrigationModePeriodicButton: MaterialButton =
            view.findViewById(R.id.irrigation_mode_periodic_button)
        val irrigationModeMoistureButton: MaterialButton =
            view.findViewById(R.id.irrigation_mode_moisture_button)
        val selectModeToggle = { mode: Mode ->
            val periodicButtonSelected = (mode == Mode.PERIODIC)
            irrigationModePeriodicButton.isChecked = periodicButtonSelected
            irrigationModeMoistureButton.isChecked = !periodicButtonSelected
            areaConfigurationMux.selectedArea.mode = mode
            // TODO: Moisture chart
        }

        val periodEdit: EditText = view.findViewById(R.id.irrigation_period_edit)
        val volumeEdit: EditText = view.findViewById(R.id.irrigation_volume_edit)
        periodEdit.addTextChangedListener {
            val text = it.toString()
            areaConfigurationMux.selectedArea.period =
                if (text.isEmpty())
                    Duration.ofDays(0)
                else
                    Duration.ofDays(text.toLong())
        }
        volumeEdit.addTextChangedListener {
            val text = it.toString()
            areaConfigurationMux.selectedArea.volume_ml = if (text.isEmpty()) 0 else text.toInt()
        }
        val updateConfigTextField = {
            // TODO: Select measurement units
            val areaCfg = areaConfigurationMux.selectedArea
            periodEdit.setText(areaCfg.period.toDays().toString())
            volumeEdit.setText(areaCfg.volume_ml.toString())
        }
        val updateAreaConfigurationViews = {
            val areaCfg = areaConfigurationMux.selectedArea
            selectModeToggle(areaCfg.mode)
            updateConfigTextField()
        }
        irrigationModePeriodicButton.setOnClickListener {
            selectModeToggle(Mode.PERIODIC)
            updateConfigTextField()
        }
        irrigationModeMoistureButton.setOnClickListener {
            selectModeToggle(Mode.MOISTURE)
            updateConfigTextField()
        }

        val areaButton1: Button = view.findViewById(R.id.irrigation_area_1_button)
        val areaButton2: Button = view.findViewById(R.id.irrigation_area_2_button)
        val areaButton3: Button = view.findViewById(R.id.irrigation_area_3_button)
        val areaButton4: Button = view.findViewById(R.id.irrigation_area_4_button)
        areaButton1.setOnClickListener {
            areaButton1.makeActive()
            areaButton2.makeInactive()
            areaButton3.makeInactive()
            areaButton4.makeInactive()
            areaConfigurationMux.selectArea(0)
            updateAreaConfigurationViews()
        }
        areaButton2.setOnClickListener {
            areaButton1.makeInactive()
            areaButton2.makeActive()
            areaButton3.makeInactive()
            areaButton4.makeInactive()
            areaConfigurationMux.selectArea(1)
            updateAreaConfigurationViews()
        }
        areaButton3.setOnClickListener {
            areaButton1.makeInactive()
            areaButton2.makeInactive()
            areaButton3.makeActive()
            areaButton4.makeInactive()
            areaConfigurationMux.selectArea(2)
            updateAreaConfigurationViews()
        }
        areaButton4.setOnClickListener {
            areaButton1.makeInactive()
            areaButton2.makeInactive()
            areaButton3.makeInactive()
            areaButton4.makeActive()
            areaConfigurationMux.selectArea(3)
            updateAreaConfigurationViews()
        }

        areaButton1.makeActive()
        areaButton2.makeInactive()
        areaButton3.makeInactive()
        areaButton4.makeInactive()
        updateAreaConfigurationViews()

        return view
    }

    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param rpcHost RPC host URL
         * @param rpcPort RPC port
         * @return A new instance of fragment IrrigationSettingsFragment.
         */
        @JvmStatic
        fun newInstance(rpcHost: String, rpcPort: Int) =
            IrrigationSettingsFragment().apply {
                arguments = Bundle().apply {
                    putString(ARG_RPC_HOST, rpcHost)
                    putInt(ARG_RPC_PORT, rpcPort)
                }
            }
    }
}
