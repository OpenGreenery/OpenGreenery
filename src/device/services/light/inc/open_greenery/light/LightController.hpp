#ifndef LIGHT_CONTROLLER_HPP
#define LIGHT_CONTROLLER_HPP

#include <memory>
#include <open_greenery/relay/IRelay.hpp>
#include <open_greenery/dataflow/light/Participants.hpp>
#include <open_greenery/dataflow/time/Participants.hpp>
#include <open_greenery/tools/LoopThread.hpp>

namespace open_greenery::light
{

class LightController
{
public:
    LightController(std::shared_ptr<open_greenery::relay::IRelay> _relay,
                    std::shared_ptr<open_greenery::dataflow::light::IConfigProvider> _config_provider,
                    std::shared_ptr<open_greenery::dataflow::time::ICurrentTimeProvider> _current_time_provider,
                    std::shared_ptr<open_greenery::dataflow::light::IManualControlProvider> _manual_control_provider,
                    std::shared_ptr<open_greenery::dataflow::light::IModeProvider> _mode_provider,
                    std::shared_ptr<open_greenery::dataflow::light::IStatusReceiver> _status_receiver);

    ~LightController();
    open_greenery::tools::FinishFuture start();
    void stop();

private:
    void LightServiceThreadFunc();
    void handleAutomaticControl();
    void handleManualControl();

    // Dependencies
    std::shared_ptr<open_greenery::relay::IRelay> m_relay;
    std::shared_ptr<open_greenery::dataflow::light::IConfigProvider> m_config_provider;
    std::shared_ptr<open_greenery::dataflow::time::ICurrentTimeProvider> m_current_time_provider;
    std::shared_ptr<open_greenery::dataflow::light::IManualControlProvider> m_manual_control_provider;
    std::shared_ptr<open_greenery::dataflow::light::IModeProvider> m_mode_provider;
    std::shared_ptr<open_greenery::dataflow::light::IStatusReceiver> m_status_receiver;

    open_greenery::dataflow::light::LightConfigRecord m_current_config;
    open_greenery::dataflow::light::Mode m_current_mode;

    std::unique_ptr<open_greenery::tools::LoopThread> m_service_thr;
};

}

#endif //LIGHT_CONTROLLER_HPP
