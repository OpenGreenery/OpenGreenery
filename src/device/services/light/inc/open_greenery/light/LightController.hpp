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
                    std::shared_ptr<open_greenery::dataflow::light::IAsyncConfigProvider> _config_provider,
                    std::shared_ptr<open_greenery::dataflow::time::ICurrentTimeProvider> _current_time_provider,
                    std::shared_ptr<open_greenery::dataflow::light::IAsyncManualControlProvider> _manual_control_provider,
                    std::shared_ptr<open_greenery::dataflow::light::IAsyncModeProvider> _mode_provider,
                    std::shared_ptr<open_greenery::dataflow::light::IAsyncStatusReceiver> _status_receiver);

    ~LightController();
    open_greenery::tools::FinishFuture start();
    void stop();

private:
    void LightServiceThreadFunc();
    void handleAutomaticControl();

    void handleConfigUpdate(open_greenery::dataflow::light::LightConfigRecord config);
    void handleManualControl(open_greenery::dataflow::light::Control control);
    void handleModeUpdate(open_greenery::dataflow::light::Mode mode);
    bool getRelayStatus();

    // Dependencies
    std::shared_ptr<open_greenery::relay::IRelay> m_relay;
    std::shared_ptr<open_greenery::dataflow::light::IAsyncConfigProvider> m_config_provider;
    std::shared_ptr<open_greenery::dataflow::time::ICurrentTimeProvider> m_current_time_provider;
    std::shared_ptr<open_greenery::dataflow::light::IAsyncManualControlProvider> m_manual_control_provider;
    std::shared_ptr<open_greenery::dataflow::light::IAsyncModeProvider> m_mode_provider;
    std::shared_ptr<open_greenery::dataflow::light::IAsyncStatusReceiver> m_status_receiver;

    open_greenery::dataflow::light::LightConfigRecord m_current_config;
    std::mutex m_config_mutex;
    open_greenery::dataflow::light::Mode m_current_mode;
    std::mutex m_mode_mutex;

    std::unique_ptr<open_greenery::tools::LoopThread> m_service_thr;
};

}

#endif //LIGHT_CONTROLLER_HPP
