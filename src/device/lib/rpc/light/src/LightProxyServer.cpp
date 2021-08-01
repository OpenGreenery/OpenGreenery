#include "open_greenery/rpc/light/LightProxyServer.hpp"
#include <cassert>
#include <grpcpp/server_builder.h>


namespace open_greenery::rpc::light
{

::grpc::Status LightProxyServer::GrpcService::GetConfig(::grpc::ServerContext * context,
                                                        const ::google::protobuf::Empty * request,
                                                        ConfigResponse * response)
{
    std::ignore = context;
    std::ignore = request;
    if (m_config)
    {
        response->set_day_start(m_config->day_start.msecsSinceStartOfDay());
        response->set_day_end(m_config->day_end.msecsSinceStartOfDay());
        m_config.reset();
    }
    else
    {
        response->clear_day_start();
        response->clear_day_end();
    }
    return {}; // OK
}

::grpc::Status LightProxyServer::GrpcService::GetManualControl(::grpc::ServerContext * context,
                                                               const ::google::protobuf::Empty * request,
                                                               ManualControlResponse * response)
{
    std::ignore = context;
    std::ignore = request;
    if (m_manual_control)
    {
        switch (*m_manual_control)
        {
            case open_greenery::dataflow::light::Control::ENABLE:
                response->set_control(ManualControlResponse::CONTROL_ENABLE);
                break;
            case open_greenery::dataflow::light::Control::DISABLE:
                response->set_control(ManualControlResponse::CONTROL_DISABLE);
                break;
            case open_greenery::dataflow::light::Control::TOGGLE:
                response->set_control(ManualControlResponse::CONTROL_TOGGLE);
                break;
            default:
                assert(false && "Unknown light::Control type");
        }
        m_manual_control.reset();
    }
    else
    {
        response->clear_control();
    }
    return {}; // OK
}

::grpc::Status LightProxyServer::GrpcService::GetMode(::grpc::ServerContext * context,
                                                      const ::google::protobuf::Empty * request,
                                                      ModeResponse * response)
{
    std::ignore = context;
    std::ignore = request;
    if (m_mode)
    {
        switch (*m_mode)
        {
            case open_greenery::dataflow::light::Mode::MANUAL:
                response->set_mode(ModeResponse::MODE_MANUAL);
                break;
            case open_greenery::dataflow::light::Mode::AUTO:
                response->set_mode(ModeResponse::MODE_AUTO);
                break;
            default:
                assert(false && "Unknown light::Mode type");
        }
        m_mode.reset();
    }
    return {}; // OK
}

::grpc::Status LightProxyServer::GrpcService::SetStatus(::grpc::ServerContext * context,
                                                        const StatusReport * request,
                                                        ::google::protobuf::Empty * response)
{
    std::ignore = context;
    std::ignore = response;
    m_status = request->is_enabled();
    return {}; // OK
}

void LightProxyServer::GrpcService::set(open_greenery::dataflow::light::LightConfigRecord record)
{
    m_config = record;
}

void LightProxyServer::GrpcService::set(open_greenery::dataflow::light::Control control)
{
    m_manual_control = control;
}

void LightProxyServer::GrpcService::set(open_greenery::dataflow::light::Mode mode)
{
    m_mode = mode;
}

std::optional<bool> LightProxyServer::GrpcService::get()
{
    auto rv = m_status;
    m_status.reset();
    return rv;
}

LightProxyServer::LightProxyServer(const std::string & host)
{
    ::grpc::ServerBuilder builder;
    builder.AddListeningPort(host, ::grpc::InsecureServerCredentials());
    builder.RegisterService(&m_service);
    m_server = builder.BuildAndStart();
}

LightProxyServer::~LightProxyServer()
{
    shutdown();
}

void LightProxyServer::set(open_greenery::dataflow::light::LightConfigRecord record)
{
    m_service.set(record);
}

void LightProxyServer::set(open_greenery::dataflow::light::Control control)
{
    m_service.set(control);
}

void LightProxyServer::set(open_greenery::dataflow::light::Mode mode)
{
    m_service.set(mode);
}

std::optional<bool> LightProxyServer::get()
{
    return m_service.get();
}

void LightProxyServer::wait()
{
    assert(m_server && "LightProxyServer wait: server is nullptr");
    m_server->Wait();
}

void LightProxyServer::shutdown()
{
    assert(m_server && "LightProxyServer shutdown: server is nullptr");
    m_server->Shutdown();
}

}
