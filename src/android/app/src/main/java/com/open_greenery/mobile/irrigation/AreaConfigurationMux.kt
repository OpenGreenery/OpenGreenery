package com.open_greenery.mobile.irrigation

import com.google.common.primitives.UnsignedInteger
import java.lang.Exception

class AreaConfigurationMux {
    private val areas = arrayOf(
        AreaConfiguration("Tomato"),
        AreaConfiguration("Peas"),
        AreaConfiguration("Mint"),
        AreaConfiguration("Avocado")
    )
    fun selectArea(index: Int) =
            if (index in 0..3)
                selectedArea = areas[index]
            else
                throw ArrayIndexOutOfBoundsException("Only 4 areas supported")

    var selectedArea: AreaConfiguration = areas[0]
        private set
}
