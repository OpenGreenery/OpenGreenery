#include "open_greenery/light/LightController.hpp"
#include <open_greenery/dataflow/time/Participants.hpp>
#include <chrono>
#include <utility>
#include <spdlog/spdlog.h>

namespace ogl = open_greenery::dataflow::light;
namespace ogt = open_greenery::dataflow::time;

namespace open_greenery::light
{

constexpr std::chrono::milliseconds THREAD_PERIOD{100u};

LightController::LightController(std::shared_ptr<open_greenery::relay::IRelay> _relay,
                                 std::shared_ptr<ogl::IConfigProvider> _config_provider,
                                 std::shared_ptr<ogt::ICurrentTimeProvider> _current_time_provider,
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

    m_current_mode = ogl::Mode::MANUAL;
    m_current_config = {QTime(), QTime()};
}

LightController::~LightController()
{
    stop();
}

open_greenery::tools::FinishFuture LightController::start()
{
    if (!m_service_thr)
    {
        m_service_thr = std::make_unique<open_greenery::tools::LoopThread>(
                [this] { LightServiceThreadFunc(); },
                THREAD_PERIOD
        );
    }
    spdlog::info("Controller start");
    return m_service_thr->start();
}

void LightController::stop()
{
    if (!m_service_thr)
    {
        return;
    }

    spdlog::info("Controller stop");
    m_service_thr->stop();
    m_service_thr.reset();
}

void LightController::LightServiceThreadFunc()
{
    // Update mode if control command
    spdlog::trace("Request mode");
    auto new_mode = m_mode_provider->get();
    if (new_mode.has_value())
    {
        m_current_mode = new_mode.value();
        spdlog::debug("Mode updated: {}", (m_current_mode == ogl::Mode::AUTO ? "AUTO" : "MANUAL"));
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
    spdlog::trace("Handle auto mode");

    auto is_event = [this](QTime some_time, QTime event_time) {
        return (some_time >= event_time) && (some_time < event_time.addMSecs(THREAD_PERIOD.count()));
    };

    // Update config if new one received
    spdlog::trace("Request config");
    auto new_config = m_config_provider->get();
    if (new_config.has_value())
    {
        m_current_config = new_config.value();
        spdlog::debug("Config updated");
    }

    spdlog::trace("Current time request");
    const auto current_time = m_current_time_provider->get();
    if (is_event(current_time, m_current_config.day_start) && !m_relay->enabled())
    {
        spdlog::info("Time to enable");
        m_relay->enable();
        // Inform receiver about status update
        spdlog::trace("Send status");
        m_status_receiver->set(m_relay->enabled());
    }
    if (is_event(current_time, m_current_config.day_end) && m_relay->enabled())
    {
        spdlog::info("Time to disable");
        m_relay->disable();
        spdlog::trace("Send status");
        m_status_receiver->set(m_relay->enabled());
    }
}

void LightController::handleManualControl()
{
    // Get control command if received
    spdlog::trace("Manual control request");
    const auto command = m_manual_control_provider->get();
    if (!command.has_value())
    {
        spdlog::trace("Manual control request");
        return;
    }

    // Handle command
    if (*command == open_greenery::dataflow::light::Control::DISABLE)
    {
        spdlog::info("Disable relay");
        m_relay->disable();
    }
    else if (*command == open_greenery::dataflow::light::Control::ENABLE)
    {
        spdlog::info("Enable relay");
        m_relay->enable();
    }
    else if (*command == open_greenery::dataflow::light::Control::TOGGLE)
    {
        spdlog::info("Toggle relay");
        m_relay->toggle();
    }

    // Inform receiver about status
    spdlog::trace("Send status");
    m_status_receiver->set(m_relay->enabled());
}

}
