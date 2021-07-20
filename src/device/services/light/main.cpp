#include <csignal>
#include <open_greenery/database/light/ControlReader.hpp>
#include <open_greenery/database/light/StatusWriter.hpp>
#include <open_greenery/gpio/OutputGPIOctl.hpp>
#include <open_greenery/light/LightController.hpp>
#include <open_greenery/light/CurrentTimeProvider.hpp>
#include <open_greenery/relay/Relay.hpp>

constexpr auto DATABASE_PATH {"/home/pi/og/db/open_greenery.db3"};
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

template <class Interface, typename ProvideT>
class OnceOptionalProvider : public Interface
{
public:
    /// Temporary stub of provider that returns data only once
    /// \param data Data which will be provided
    // TODO: Implement providers for data/control from user
    explicit OnceOptionalProvider(ProvideT data)
        :m_data_to_provide(data) {}

    // Interface
    std::optional<ProvideT> get() override
    {
        auto rv = m_data_to_provide;
        m_data_to_provide.reset();
        return rv;
    }

private:
    std::optional<ProvideT> m_data_to_provide;
};

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
    auto config_provider =
            std::make_shared<OnceOptionalProvider<
                    open_greenery::dataflow::light::IConfigProvider,
                    open_greenery::dataflow::light::LightConfigRecord>>(config);

    // time provider
    auto time_provider = std::make_shared<open_greenery::light::CurrentTimeProvider>();

    // database
    auto db = std::make_shared<SQLite::Database>(DATABASE_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    // manual control
    auto ctl_reader = std::make_shared<open_greenery::database::light::ControlReader>(db);
    // light status
    auto status_writer = std::make_shared<open_greenery::database::light::StatusWriter>(db);

    // mode provider
    auto mode_provider =
            std::make_shared<OnceOptionalProvider<
                    open_greenery::dataflow::light::IModeProvider,
                    open_greenery::dataflow::light::Mode>>(open_greenery::dataflow::light::Mode::MANUAL);


    // light controller
    s_controller.emplace(
            light_relay,
            config_provider,
            time_provider,
            ctl_reader,
            mode_provider,
            status_writer
        );
    s_controller_finish = s_controller->start();
    if (s_controller_finish->valid())
    {
        s_controller_finish->wait();  // Execute service until termination signal
    }


    return 0;
}
