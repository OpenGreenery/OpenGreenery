package com.open_greenery.mobile

import com.google.protobuf.Empty
import io.grpc.okhttp.OkHttpChannelBuilder
import java.time.LocalTime

class RelayRpcClient(address: String, port: Int) {
    private var channel = OkHttpChannelBuilder
        .forAddress(address, port)
        .usePlaintext()
        .build()
    private var stub = RelayGrpc.newBlockingStub(channel)

    fun enable() {
        manualControl(RelayProto.ManualControlRequest.Control.CONTROL_ENABLE)
    }

    fun disable() {
        manualControl(RelayProto.ManualControlRequest.Control.CONTROL_DISABLE)
    }

    fun toggle() {
        manualControl(RelayProto.ManualControlRequest.Control.CONTROL_TOGGLE)
    }

    val service_status: ServiceStatus
        get() = getServiceStatus()

    fun getServiceStatus(): ServiceStatus {
        val service_status: RelayProto.ServiceStatus
            = stub.getServiceStatus(Empty.getDefaultInstance())
        val mode: Mode =
            if (service_status.modeSettings.mode == RelayProto.ModeSetting.Mode.MODE_AUTO)
                Mode.AUTO
            else
                Mode.MANUAL
        val relayEnabled = service_status.relayStatus.isEnabled
        // Milliseconds from day start to seconds
        val dayStart = LocalTime.ofSecondOfDay(
            service_status.config.dayStart
                .div(1000).toLong())
        val dayEnd = LocalTime.ofSecondOfDay(
            service_status.config.dayEnd
                .div(1000).toLong())
        return ServiceStatus(
            mode,
            relayEnabled,
            Config(dayStart, dayEnd)
        )
    }

    var config: Config
        get() = service_status.config
        set(value) = updateConfig(value)

    fun updateConfig(config: Config) {
        val proto_config = RelayProto.Config.newBuilder()
            .setDayStart(config.dayStart.toSecondOfDay() * 1000)
            .setDayEnd(config.dayEnd.toSecondOfDay() * 1000)
            .build()
        stub.setConfig(proto_config)
    }

    fun updateDayStart(dayStart: LocalTime) {
        config = Config(dayStart, config.dayEnd)
    }

    fun updateDayEnd(dayEnd: LocalTime) {
        config = Config(config.dayStart, dayEnd)
    }

    var mode: Mode
        get() = service_status.mode
        set(m) {
            val protoMode =
                if (m == Mode.MANUAL)
                    RelayProto.ModeSetting.Mode.MODE_MANUAL
                else
                    RelayProto.ModeSetting.Mode.MODE_AUTO
            stub.setMode(RelayProto.ModeSetting.newBuilder().setMode(protoMode).build())
        }


    private fun manualControl(control: RelayProto.ManualControlRequest.Control) {
        val request = RelayProto.ManualControlRequest
            .newBuilder()
            .setControl(control)
            .build()
        stub.manualControl(request)
    }

}
