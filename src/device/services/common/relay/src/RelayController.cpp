#include "open_greenery/relay/RelayController.hpp"
#include <chrono>
#include <spdlog/spdlog.h>

namespace ogrc = open_greenery::dataflow::relay;
namespace ogdft = open_greenery::dataflow::time;

namespace open_greenery::relay
{

constexpr std::chrono::milliseconds THREAD_PERIOD{100u};

RelayController::RelayController(
        std::shared_ptr<open_greenery::relay::IRelay> _relay,
        std::shared_ptr<ogrc::IAsyncConfigProvider> _config_provider,
        std::shared_ptr<ogdft::ICurrentTimeProvider> _current_time_provider,
        std::shared_ptr<ogrc::IAsyncManualControlProvider> _manual_control_provider,
        std::shared_ptr<ogrc::IAsyncModeProvider> _mode_provider,
        std::shared_ptr<ogrc::IAsyncRelayStatusReceiver> _relay_status_receiver,
        std::shared_ptr<ogrc::IAsyncServiceStatusReceiver> _service_status_receiver
        )
        : m_relay(std::move(_relay)),
          m_config_provider(std::move(_config_provider)),
          m_current_time_provider(std::move(_current_time_provider)),
          m_manual_control_provider(std::move(_manual_control_provider)),
          m_mode_provider(std::move(_mode_provider)),
          m_relay_status_receiver(std::move(_relay_status_receiver)),
          m_service_status_receiver(std::move(_service_status_receiver))
{
    assert(m_relay);
    assert(m_config_provider);
    assert(m_current_time_provider);
    assert(m_manual_control_provider);
    assert(m_mode_provider);
    assert(m_relay_status_receiver);

    std::lock_guard<std::mutex> mode_lock(m_mode_mutex);
    std::lock_guard<std::mutex> config_lock(m_config_mutex);
    m_current_mode = ogrc::Mode::MANUAL;
    m_current_config = {QTime(), QTime()};

    m_config_provider->onUpdate([this](ogrc::Config c){ handleConfigUpdate(c); });
    m_manual_control_provider->onUpdate([this](ogrc::Control c) { handleManualControl(c); });
    m_mode_provider->onUpdate([this](ogrc::Mode m){ handleModeUpdate(m); });
    m_relay_status_receiver->onRequest([this] { return getRelayStatus(); });
    m_service_status_receiver->onRequest([this] { return getServiceStatus(); });
}

RelayController::~RelayController()
{
    stop();
}

open_greenery::tools::FinishFuture RelayController::start()
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

void RelayController::stop()
{
    if (!m_service_thr)
    {
        return;
    }

    spdlog::info("Controller stop");
    m_service_thr->stop();
    m_service_thr.reset();
}

void RelayController::LightServiceThreadFunc()
{
    std::lock_guard<std::mutex> l (m_mode_mutex);
    if (m_current_mode == ogrc::Mode::AUTO)
    {
        handleAutomaticControl();
    }
}

void RelayController::handleAutomaticControl()
{
    spdlog::trace("Handle auto mode");

    auto is_event = [this](QTime some_time, QTime event_time) {
        return (some_time >= event_time) && (some_time < event_time.addMSecs(THREAD_PERIOD.count()));
    };

    spdlog::trace("Current time request");
    const auto current_time = m_current_time_provider->get();
    std::lock_guard<std::mutex> l (m_config_mutex);
    if (is_event(current_time, m_current_config.day_start) && !m_relay->enabled())
    {
        spdlog::info("Time to enable");
        m_relay->enable();
    }
    if (is_event(current_time, m_current_config.day_end) && m_relay->enabled())
    {
        spdlog::info("Time to disable");
        m_relay->disable();
    }
}

bool RelayController::getRelayStatus()
{
    const auto status = m_relay->enabled();
    spdlog::debug("Send status: {}", (status ? "enabled" : "disabled"));
    return status;
}

void RelayController::handleManualControl(open_greenery::dataflow::relay::Control control)
{
    // Handle command
    if (control == open_greenery::dataflow::relay::Control::DISABLE)
    {
        spdlog::info("Disable relay");
        m_relay->disable();
    }
    else if (control == open_greenery::dataflow::relay::Control::ENABLE)
    {
        spdlog::info("Enable relay");
        m_relay->enable();
    }
    else if (control == open_greenery::dataflow::relay::Control::TOGGLE)
    {
        spdlog::info("Toggle relay");
        m_relay->toggle();
    }
}

void RelayController::handleModeUpdate(open_greenery::dataflow::relay::Mode mode)
{
    std::lock_guard<std::mutex> l (m_mode_mutex);
    spdlog::info("Mode changed: {}",
                 (mode == open_greenery::dataflow::relay::Mode::MANUAL ? "MANUAL" : "AUTO"));
    m_current_mode = mode;
}

void RelayController::handleConfigUpdate(open_greenery::dataflow::relay::Config config)
{
    auto TimeStr = [](const QTime & t){return t.toString("hh:mm:ss").toStdString();};
    spdlog::info("Relay configuration updated: start={}, end={}",
                 TimeStr(config.day_start),
                 TimeStr(config.day_end));
    std::lock_guard<std::mutex> l (m_config_mutex);
    m_current_config = config;
}

open_greenery::dataflow::relay::ServiceStatus RelayController::getServiceStatus()
{
    ogrc::ServiceStatus service_status;
    std::scoped_lock l {m_mode_mutex, m_config_mutex};
    service_status.mode = m_current_mode;
    service_status.relay_enabled = m_relay->enabled();
    service_status.config = m_current_config;
    return service_status;
}

}
