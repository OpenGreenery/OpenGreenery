#ifndef LIGHT_CONTROLLER_HPP
#define LIGHT_CONTROLLER_HPP

#include <memory>
#include <open_greenery/relay/IRelay.hpp>
#include <open_greenery/dataflow/relay/Participants.hpp>
#include <open_greenery/dataflow/time/Participants.hpp>
#include <open_greenery/tools/LoopThread.hpp>

namespace open_greenery::relay
{

class RelayController
{
public:
    RelayController(
            std::shared_ptr<open_greenery::relay::IRelay> _relay,
            std::shared_ptr<open_greenery::dataflow::relay::IAsyncConfigProvider> _config_provider,
            std::shared_ptr<open_greenery::dataflow::time::ICurrentTimeProvider> _current_time_provider,
            std::shared_ptr<open_greenery::dataflow::relay::IAsyncManualControlProvider> _manual_control_provider,
            std::shared_ptr<open_greenery::dataflow::relay::IAsyncModeProvider> _mode_provider,
            std::shared_ptr<open_greenery::dataflow::relay::IAsyncRelayStatusReceiver> _relay_status_receiver,
            std::shared_ptr<open_greenery::dataflow::relay::IAsyncServiceStatusReceiver> _service_status_receiver
    );

    ~RelayController();
    open_greenery::tools::FinishFuture start();
    void stop();

private:
    void LightServiceThreadFunc();
    void handleAutomaticControl();

    void handleConfigUpdate(open_greenery::dataflow::relay::Config config);
    void handleManualControl(open_greenery::dataflow::relay::Control control);
    void handleModeUpdate(open_greenery::dataflow::relay::Mode mode);
    bool getRelayStatus();
    open_greenery::dataflow::relay::ServiceStatus getServiceStatus();

    // Dependencies
    std::shared_ptr<open_greenery::relay::IRelay> m_relay;
    std::shared_ptr<open_greenery::dataflow::relay::IAsyncConfigProvider> m_config_provider;
    std::shared_ptr<open_greenery::dataflow::time::ICurrentTimeProvider> m_current_time_provider;
    std::shared_ptr<open_greenery::dataflow::relay::IAsyncManualControlProvider> m_manual_control_provider;
    std::shared_ptr<open_greenery::dataflow::relay::IAsyncModeProvider> m_mode_provider;
    std::shared_ptr<open_greenery::dataflow::relay::IAsyncRelayStatusReceiver> m_relay_status_receiver;
    std::shared_ptr<open_greenery::dataflow::relay::IAsyncServiceStatusReceiver> m_service_status_receiver;

    open_greenery::dataflow::relay::Config m_current_config;
    std::mutex m_config_mutex;
    open_greenery::dataflow::relay::Mode m_current_mode;
    std::mutex m_mode_mutex;

    std::unique_ptr<open_greenery::tools::LoopThread> m_service_thr;
};

}

#endif //LIGHT_CONTROLLER_HPP
