#include <csignal>
#include <open_greenery/database/light/ControlHandledWriter.hpp>
#include <open_greenery/database/light/ControlReader.hpp>
#include <open_greenery/database/light/StatusWriter.hpp>
#include <open_greenery/gpio/OutputGPIOctl.hpp>
#include <open_greenery/light/LightController.hpp>
#include <open_greenery/light/CurrentTimeProvider.hpp>
#include <open_greenery/relay/Relay.hpp>

static std::unique_ptr<open_greenery::light::LightController> s_controller;

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
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
    // relay
    constexpr std::uint8_t LIGHT_PIN_NUMBER {2u};
    constexpr open_greenery::gpio::PinId LIGHT_PIN_ID {open_greenery::gpio::PinNumber(LIGHT_PIN_NUMBER),
                                                       open_greenery::gpio::Pinout::WIRING_PI};
    auto light_pin = open_greenery::gpio::GPIOFactory::getInstance().getOutputGPIOctl(LIGHT_PIN_ID);
    auto light_relay = std::make_shared<open_greenery::relay::Relay>(light_pin);

    // config
    const QTime DAY_START_TIME (8, 00);
    const QTime DAY_END_TIME (22, 00);
    const open_greenery::dataflow::light::LightConfigRecord config {DAY_START_TIME, DAY_END_TIME};

    // manual control
    auto ctl_reader = std::make_shared<open_greenery::database::light::ControlReader>();
    auto ctl_handled_writer = std::make_shared<open_greenery::database::light::ControlHandledWriter>();

    // light status
    auto status_writer = std::make_shared<open_greenery::database::light::StatusWriter>();

    // time provider
    auto time_provider = std::make_shared<open_greenery::light::CurrentTimeProvider>();


    // light controller
    s_controller = std::make_unique<open_greenery::light::LightController>(
            light_relay,
            config,
            ctl_reader,
            ctl_handled_writer,
            status_writer,
            time_provider
        );
    s_controller->start();

    while (true) {} // Execute service until termination signal
    return 0;
}
