package com.open_greenery.mobile.irrigation

import java.time.Duration

class AreaConfiguration(val name: String) {
    var mode: Mode = Mode.PERIODIC
    var period: Duration
        get() = if (mode == Mode.PERIODIC) periodic_period else moisture_period
        set(value) {
            if (mode == Mode.PERIODIC)
                periodic_period = value
            else
                moisture_period = value
        }
    var volume_ml: Int
        get() = if (mode == Mode.PERIODIC) periodic_volume_ml else moisture_volume_ml
        set(value) {
            if (mode == Mode.PERIODIC)
                periodic_volume_ml = value
            else
                moisture_volume_ml = value
        }

    private var periodic_period = Duration.ofDays(4)
    private var periodic_volume_ml: Int = 500

    private var moisture_period = Duration.ofMinutes(5)
    private var moisture_volume_ml: Int = 50
}
