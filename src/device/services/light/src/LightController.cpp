#include "open_greenery/light/LightController.hpp"
#include <chrono>
#include <utility>

namespace ogl = open_greenery::dataflow::light;

namespace open_greenery::light
{

constexpr std::chrono::milliseconds THREAD_PERIOD{100u};

LightController::LightController(std::shared_ptr<open_greenery::relay::IRelay> _relay,
                                 std::shared_ptr<ogl::IConfigProvider> _config_provider,
                                 std::shared_ptr<ogl::ICurrentTimeProvider> _current_time_provider,
                                 std::shared_ptr<ogl::IManualControlProvider> _manual_control_provider,
                                 std::shared_ptr<ogl::IModeProvider> _mode_provider,
                                 std::shared_ptr<ogl::IStatusReceiver> _status_receiver)
        : m_relay(std::move(_relay)),
          m_config_provider(std::move(_config_provider)),
          m_current_time_provider(std::move(_current_time_provider)),
          m_manual_control_provider(std::move(_manual_control_provider)),
          m_mode_provider(std::move(_mode_provider)),
          m_status_receiver(std::move(_status_receiver))
{
    assert(m_relay);
    assert(m_config_provider);
    assert(m_current_time_provider);
    assert(m_manual_control_provider);
    assert(m_mode_provider);
    assert(m_status_receiver);
}

LightController::~LightController()
{
    stop();
}

void LightController::start()
{
    if (!m_service_thr)
    {
        m_service_thr = std::make_unique<open_greenery::tools::LoopThread>(
                [this] { LightServiceThreadFunc(); },
                THREAD_PERIOD
        );
    }
    m_service_thr->start();
}

void LightController::stop()
{
    if (m_service_thr)
    {
        m_service_thr->stop();
    }
}

void LightController::LightServiceThreadFunc()
{
    // Update mode if control command received
    auto new_mode = m_mode_provider->get();
    if (new_mode.has_value())
    {
        m_current_mode = new_mode.value();
    }

    if (m_current_mode == ogl::Mode::AUTO)
    {
        handleAutomaticControl();
    }
    // Handle manual control commands in both modes
    handleManualControl();
}

void LightController::handleAutomaticControl()
{
    auto is_event = [this](QTime some_time, QTime event_time) {
        return (some_time >= event_time) && (some_time < event_time.addMSecs(THREAD_PERIOD.count()));
    };

    // Update config if new one received
    auto new_config = m_config_provider->get();
    if (new_config.has_value())
    {
        m_current_config = new_config.value();
    }

    const auto current_time = m_current_time_provider->get();
    if (is_event(current_time, m_current_config.day_start))
    {
        m_relay->enable();
    }
    else if (is_event(current_time, m_current_config.day_end))
    {
        m_relay->disable();
    }

    // Inform receiver about status
    m_status_receiver->set(m_relay->enabled());
}

void LightController::handleManualControl()
{
    // Get control command if received
    const auto command = m_manual_control_provider->get();
    if (!command.has_value())
    {
        return;
    }

    // Handle command
    if (*command == open_greenery::dataflow::light::Control::DISABLE)
    {
        m_relay->disable();
    }
    else if (*command == open_greenery::dataflow::light::Control::ENABLE)
    {
        m_relay->enable();
    }
    else if (*command == open_greenery::dataflow::light::Control::TOGGLE)
    {
        m_relay->toggle();
    }

    // Inform receiver about status
    m_status_receiver->set(m_relay->enabled());
}

}
