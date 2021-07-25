#include "open_greenery/rpc/light/LightProxyServer.hpp"
#include <cassert>
#include <optional>


namespace open_greenery::rpc::light
{


::grpc::Status LightProxyServer::GetConfig(::grpc::ServerContext *context,
                                           const ::google::protobuf::Empty *request,
                                           ::open_greenery::rpc::light::ConfigResponse *response)
{
    std::ignore = context;
    std::ignore = request;
    if (m_config)
    {
        response->set_day_start(m_config->day_start.msecsSinceStartOfDay());
        response->set_day_end(m_config->day_end.msecsSinceStartOfDay());
    }
    else
    {
        response->clear_day_start();
        response->clear_day_end();
    }
    return {}; // OK
}

::grpc::Status LightProxyServer::GetManualControl(::grpc::ServerContext *context,
                                                  const ::google::protobuf::Empty *request,
                                                  ::open_greenery::rpc::light::ManualControlResponse *response)
{
    std::ignore = context;
    std::ignore = request;
    if (m_manual_control)
    {
        switch (*m_manual_control)
        {
            case open_greenery::dataflow::light::Control::ENABLE:
                response->set_control(open_greenery::rpc::light::ManualControlResponse::CONTROL_ENABLE);
                break;
            case open_greenery::dataflow::light::Control::DISABLE:
                response->set_control(open_greenery::rpc::light::ManualControlResponse::CONTROL_DISABLE);
                break;
            case open_greenery::dataflow::light::Control::TOGGLE:
                response->set_control(open_greenery::rpc::light::ManualControlResponse::CONTROL_TOGGLE);
                break;
            default:
                assert(false && "Unknown light::Control type");
        }
    }
    else
    {
        response->clear_control();
    }
    return {}; // OK
}

::grpc::Status LightProxyServer::GetMode(::grpc::ServerContext *context,
                                         const ::google::protobuf::Empty *request,
                                         ::open_greenery::rpc::light::ModeResponse *response)
{
    std::ignore = context;
    std::ignore = request;
    if (m_mode)
    {
        switch (*m_mode)
        {
            case open_greenery::dataflow::light::Mode::MANUAL:
                response->set_mode(open_greenery::rpc::light::ModeResponse::MODE_MANUAL);
                break;
            case open_greenery::dataflow::light::Mode::AUTO:
                response->set_mode(open_greenery::rpc::light::ModeResponse::MODE_AUTO);
                break;
            default:
                assert(false && "Unknown light::Mode type");
        }
    }
    return {}; // OK
}

::grpc::Status LightProxyServer::SetStatus(::grpc::ServerContext *context,
                                           const ::open_greenery::rpc::light::StatusReport *request,
                                           ::google::protobuf::Empty *response)
{
    std::ignore = context;
    std::ignore = response;
    m_status = request->is_enabled();
    return {}; // OK
}

void LightProxyServer::set(open_greenery::dataflow::light::LightConfigRecord record)
{
    m_config = record;
}

void LightProxyServer::set(open_greenery::dataflow::light::Control control)
{
    m_manual_control = control;
}

void LightProxyServer::set(open_greenery::dataflow::light::Mode mode)
{
    m_mode = mode;
}

std::optional<bool> LightProxyServer::get()
{
    auto rv = m_status;
    m_status.reset();
    return rv;
}

}
