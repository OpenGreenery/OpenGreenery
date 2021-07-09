#include "open_greenery/light/LightController.hpp"
#include <chrono>
#include <utility>


namespace open_greenery::light
{
constexpr std::chrono::milliseconds THREAD_PERIOD{100u};

LightController::LightController(std::shared_ptr<open_greenery::relay::IRelay> _relay,
                                 ogl::LightConfigRecord _config,
                                 std::shared_ptr<ogl::IControlProvider> _manual_control,
                                 std::shared_ptr<ogl::IControlHandledReceiver> _manual_control_handled_receiver,
                                 std::shared_ptr<ogl::IStatusReceiver> _status_receiver,
                                 std::shared_ptr<ogl::ITimeProvider> _current_time_provider)
        :m_relay(std::move(_relay)),
        m_config(_config),
        m_manual_control(std::move(_manual_control)),
        m_manual_control_handled_receiver(std::move(_manual_control_handled_receiver)),
        m_status_receiver(std::move(_status_receiver)),
        m_current_time_provider(std::move(_current_time_provider))
{}

LightController::~LightController()
{
    stop();
}

void LightController::start()
{
    if (!m_service_thr)
    {
        m_service_thr = std::make_unique<open_greenery::tools::LoopThread>(
                [this]{LightServiceThreadFunc();},
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
    handleAutomaticControl();
    handleManualControl();
    m_status_receiver->set(m_relay->enabled());
}

void LightController::handleAutomaticControl()
{
    auto is_event = [this](QTime some_time, QTime event_time){
        return (some_time >= event_time) && (some_time < event_time.addMSecs(THREAD_PERIOD.count()));
    };

    const auto current_time = m_current_time_provider->get();
    if (is_event(current_time, m_config.day_start))
    {
        m_relay->enable();
    }
    else if (is_event(current_time, m_config.day_end))
    {
        m_relay->disable();
    }
}

void LightController::handleManualControl()
{
    // Get control command
    const auto flag = m_manual_control->get();
    if (!flag.has_value())
    {
        return;
    }

    // Handle control
    const auto control = flag.value();
    if (control == open_greenery::dataflow::light::Control::DISABLE)
    {
        m_relay->disable();
    }
    else if (control == open_greenery::dataflow::light::Control::ENABLE)
    {
        m_relay->enable();
    }
    else if (control == open_greenery::dataflow::light::Control::TOGGLE)
    {
        m_relay->toggle();
    }

    // Set control-handled flag in db
    m_manual_control_handled_receiver->handled();
}

}
