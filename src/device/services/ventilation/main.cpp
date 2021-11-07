#include <csignal>
#include <open_greenery/gpio/OutputGPIOctl.hpp>
#include <open_greenery/relay/RelayController.hpp>
#include <open_greenery/relay/CurrentTimeProvider.hpp>
#include <open_greenery/relay/Relay.hpp>
#include <spdlog/cfg/env.h>
#include <open_greenery/rpc/relay/Server.hpp>

static std::optional<open_greenery::relay::RelayController> s_controller;
static std::optional<open_greenery::tools::FinishFuture> s_controller_finish;

void signalHandler(int signal)
{
    if (s_controller)
    {
        s_controller->stop();
    }
    exit(signal);
}

int main()
{
    spdlog::cfg::load_env_levels();

    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
    // relay
    constexpr std::uint8_t VENTILATION_PIN_NUMBER {7u};
    constexpr open_greenery::gpio::PinId VENTILATION_PIN_ID {open_greenery::gpio::PinNumber(VENTILATION_PIN_NUMBER),
                                                             open_greenery::gpio::Pinout::WIRING_PI};
    auto ventilation_pin = open_greenery::gpio::GPIOFactory::getInstance().getOutputGPIOctl(VENTILATION_PIN_ID);
    auto ventilation_relay = std::make_shared<open_greenery::relay::Relay>(ventilation_pin);

    // time provider
    auto time_provider = std::make_shared<open_greenery::relay::CurrentTimeProvider>();

    // rpc client
    constexpr char RPC_HOST [] {"localhost:8091"};
    auto rpc_server = std::make_shared<open_greenery::rpc::relay::Server>(RPC_HOST);

    // ventilation controller
    s_controller.emplace(
            ventilation_relay,
            rpc_server,
            time_provider,
            rpc_server,
            rpc_server,
            rpc_server
        );
    s_controller_finish = s_controller->start();
    if (s_controller_finish->valid())
    {
        s_controller_finish->wait();  // Execute service until termination signal
    }

    return 0;
}
