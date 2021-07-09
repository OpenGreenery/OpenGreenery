#ifndef LIGHT_CONTROLLER_HPP
#define LIGHT_CONTROLLER_HPP

#include <memory>
#include "open_greenery/relay/IRelay.hpp"
#include "open_greenery/dataflow/light/IControlProvider.hpp"
#include "open_greenery/dataflow/light/IControlHandledReceiver.hpp"
#include "open_greenery/dataflow/light/IStatusReceiver.hpp"
#include "open_greenery/dataflow/light/ITimeProvider.hpp"
#include "open_greenery/dataflow/light/ConfigRecord.hpp"
#include <open_greenery/tools/LoopThread.hpp>

namespace open_greenery::light
{

namespace ogl = open_greenery::dataflow::light;

class LightController
{
public:
    LightController(std::shared_ptr<open_greenery::relay::IRelay> _relay,
                    ogl::LightConfigRecord _config,
                    std::shared_ptr<ogl::IControlProvider> _manual_control,
                    std::shared_ptr<ogl::IControlHandledReceiver> _manual_control_handled_receiver,
                    std::shared_ptr<ogl::IStatusReceiver> _status_receiver,
                    std::shared_ptr<ogl::ITimeProvider> _current_time_provider);

    ~LightController();
    void start();
    void stop();

private:
    void LightServiceThreadFunc();
    void handleAutomaticControl();
    void handleManualControl();

    std::shared_ptr<open_greenery::relay::IRelay> m_relay;
    ogl::LightConfigRecord m_config;
    std::shared_ptr<ogl::IControlProvider> m_manual_control;
    std::shared_ptr<ogl::IControlHandledReceiver> m_manual_control_handled_receiver;
    std::shared_ptr<ogl::IStatusReceiver> m_status_receiver;
    std::shared_ptr<ogl::ITimeProvider> m_current_time_provider;

    std::unique_ptr<open_greenery::tools::LoopThread> m_service_thr;
};

}

#endif //LIGHT_CONTROLLER_HPP
