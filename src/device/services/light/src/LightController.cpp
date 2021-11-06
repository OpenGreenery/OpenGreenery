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
                                 std::shared_ptr<ogl::IAsyncConfigProvider> _config_provider,
                                 std::shared_ptr<ogt::ICurrentTimeProvider> _current_time_provider,
                                 std::shared_ptr<ogl::IAsyncManualControlProvider> _manual_control_provider,
                                 std::shared_ptr<ogl::IAsyncModeProvider> _mode_provider,
                                 std::shared_ptr<ogl::IAsyncStatusRecevier> _status_receiver)
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

    std::lock_guard<std::mutex> mode_lock(m_mode_mutex);
    std::lock_guard<std::mutex> config_lock(m_config_mutex);
    m_current_mode = ogl::Mode::MANUAL;
    m_current_config = {QTime(), QTime()};

    m_config_provider->onUpdate([this](ogl::LightConfigRecord c){ handleConfigUpdate(c); });
    m_manual_control_provider->onUpdate([this](ogl::Control c) { handleManualControl(c); });
    m_mode_provider->onUpdate([this](ogl::Mode m){ handleModeUpdate(m); });
    m_status_receiver->onRequest([this] { return getRelayStatus(); });
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
    std::lock_guard<std::mutex> l (m_mode_mutex);
    if (m_current_mode == ogl::Mode::AUTO)
    {
        handleAutomaticControl();
    }
}

void LightController::handleAutomaticControl()
{
    spdlog::trace("Handle auto mode");

    auto is_event = [this](QTime some_time, QTime event_time) {
        return (some_time >= event_time) && (some_time < event_time.addMSecs(THREAD_PERIOD.count()));
    };

    spdlog::trace("Current time request");
    const auto current_time = m_current_time_provider->get();
    std::lock_guard<std::mutex> l (m_config_mutex);
    if (is_event(current_time, m_current_config.day_start))
    {
        spdlog::info("Time to enable");
        m_relay->enable();
    }
    if (is_event(current_time, m_current_config.day_end))
    {
        spdlog::info("Time to disable");
        m_relay->disable();
    }
}

bool LightController::getRelayStatus()
{
    const auto status = m_relay->enabled();
    spdlog::debug("Send status: {}", (status ? "enabled" : "disabled"));
    return status;
}

void LightController::handleManualControl(open_greenery::dataflow::light::Control control)
{
    // Handle command
    if (control == open_greenery::dataflow::light::Control::DISABLE)
    {
        spdlog::info("Disable relay");
        m_relay->disable();
    }
    else if (control == open_greenery::dataflow::light::Control::ENABLE)
    {
        spdlog::info("Enable relay");
        m_relay->enable();
    }
    else if (control == open_greenery::dataflow::light::Control::TOGGLE)
    {
        spdlog::info("Toggle relay");
        m_relay->toggle();
    }
}

void LightController::handleModeUpdate(open_greenery::dataflow::light::Mode mode)
{
    std::lock_guard<std::mutex> l (m_mode_mutex);
    spdlog::info("Mode changed: {}",
                 (mode == open_greenery::dataflow::light::Mode::MANUAL ? "MANUAL" : "AUTO"));
    m_current_mode = mode;
}

void LightController::handleConfigUpdate(open_greenery::dataflow::light::LightConfigRecord config)
{
    auto TimeStr = [](const QTime & t){return t.toString("hh:mm:ss").toStdString();};
    spdlog::info("Relay configuration updated: start={}, end={}",
                 TimeStr(config.day_start),
                 TimeStr(config.day_end));
    std::lock_guard<std::mutex> l (m_config_mutex);
    m_current_config = config;
}

}
