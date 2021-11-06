#include <csignal>
#include <open_greenery/rpc/relay/Client.hpp>
#include <open_greenery/gpio/OutputGPIOctl.hpp>
#include <open_greenery/light/LightController.hpp>
#include <open_greenery/light/CurrentTimeProvider.hpp>
#include <open_greenery/relay/Relay.hpp>
#include <spdlog/cfg/env.h>
#include <open_greenery/rpc/relay/Server.hpp>

static std::optional<open_greenery::light::LightController> s_controller;
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
    constexpr std::uint8_t LIGHT_PIN_NUMBER {6u};
    constexpr open_greenery::gpio::PinId LIGHT_PIN_ID {open_greenery::gpio::PinNumber(LIGHT_PIN_NUMBER),
                                                       open_greenery::gpio::Pinout::WIRING_PI};
    auto light_pin = open_greenery::gpio::GPIOFactory::getInstance().getOutputGPIOctl(LIGHT_PIN_ID);
    auto light_relay = std::make_shared<open_greenery::relay::Relay>(light_pin);

    // time provider
    auto time_provider = std::make_shared<open_greenery::light::CurrentTimeProvider>();

    // rpc client
    constexpr char RPC_HOST [] {"localhost:8090"};
    auto rpc_server = std::make_shared<open_greenery::rpc::relay::Server>(RPC_HOST);

    // light controller
    s_controller.emplace(
            light_relay,
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
