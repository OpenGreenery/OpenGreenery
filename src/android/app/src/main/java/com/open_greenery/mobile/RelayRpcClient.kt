package com.open_greenery.mobile

import io.grpc.okhttp.OkHttpChannelBuilder

class RelayRpcClient(address: String, port: Int) {
    private var channel = OkHttpChannelBuilder
        .forAddress(address, port)
        .usePlaintext()
        .build()
    private var stub = RelayGrpc.newBlockingStub(channel)

    fun enable() {
        manualControl(OpenGreeneryRelayProto.ManualControlRequest.Control.CONTROL_ENABLE)
    }

    fun disable() {
        manualControl(OpenGreeneryRelayProto.ManualControlRequest.Control.CONTROL_DISABLE)
    }

    fun toggle() {
        manualControl(OpenGreeneryRelayProto.ManualControlRequest.Control.CONTROL_TOGGLE)
    }

    private fun manualControl(control: OpenGreeneryRelayProto.ManualControlRequest.Control) {
        val request = OpenGreeneryRelayProto.ManualControlRequest
            .newBuilder()
            .setControl(control)
            .build()
        val response = stub.manualControl(request)
    }

}
