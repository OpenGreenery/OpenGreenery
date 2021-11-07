#include "open_greenery/rpc/relay/Server.hpp"
#include <cassert>
#include <utility>
#include <grpcpp/server_builder.h>
#include <spdlog/spdlog.h>
#include <google/protobuf/empty.pb.h>


namespace open_greenery::rpc::relay
{

grpc::Status Server::Service::SetConfig(
        grpc::ServerContext * context,
        const open_greenery::rpc::relay::Config * request,
        google::protobuf::Empty * response)
{
    std::ignore = context;
    std::ignore = response;
    open_greenery::dataflow::relay::Config received_config{
            QTime::fromMSecsSinceStartOfDay(request->day_start()),
            QTime::fromMSecsSinceStartOfDay(request->day_end())
    };

    auto TimeStr = [](const QTime & t) { return t.toString("hh:mm:ss").toStdString(); };
    spdlog::debug("Relay config received: day start={}, end={}",
                  TimeStr(received_config.day_start),
                  TimeStr(received_config.day_end));

    m_config_update_handler(received_config);
    return {}; // OK
}

grpc::Status Server::Service::ManualControl(
        grpc::ServerContext * context,
        const open_greenery::rpc::relay::ManualControlRequest * request,
        google::protobuf::Empty * response)
{
    std::ignore = context;
    std::ignore = response;

    open_greenery::dataflow::relay::Control control;

    switch (request->control())
    {
        case ManualControlRequest::CONTROL_ENABLE:
            spdlog::debug("Manual enable received");
            control = open_greenery::dataflow::relay::Control::ENABLE;
            break;
        case ManualControlRequest::CONTROL_DISABLE:
            spdlog::debug("Manual disable received");
            control = open_greenery::dataflow::relay::Control::DISABLE;
            break;
        case ManualControlRequest::CONTROL_TOGGLE:
            spdlog::debug("Manual toggle received");
            control = open_greenery::dataflow::relay::Control::TOGGLE;
            break;
        default:
            assert(false && "Unknown relay::Control type");
            // Fictive assignment to avoid warning
            control = open_greenery::dataflow::relay::Control::DISABLE;
    }
    m_manual_control_handler(control);

    return {};
}

grpc::Status Server::Service::SetMode(
        grpc::ServerContext * context,
        const open_greenery::rpc::relay::ModeSetting * request,
        google::protobuf::Empty * response)
{
    std::ignore = context;
    std::ignore = response;

    open_greenery::dataflow::relay::Mode mode;

    switch (request->mode())
    {
        case ModeSetting::MODE_MANUAL:
            spdlog::debug("Manual mode setting received");
            mode = open_greenery::dataflow::relay::Mode::MANUAL;
            break;
        case ModeSetting::MODE_AUTO:
            spdlog::debug("Auto mode setting received");
            mode = open_greenery::dataflow::relay::Mode::AUTO;
            break;
        default:
            assert(false && "relay relay::Mode type");
            // Fictive assignment to avoid warning
            mode = open_greenery::dataflow::relay::Mode::MANUAL;
    }

    m_mode_update_handler(mode);

    return {}; // OK
}

grpc::Status Server::Service::GetStatus(
        grpc::ServerContext * context,
        const google::protobuf::Empty * request,
        open_greenery::rpc::relay::Status * response)
{
    std::ignore = context;
    std::ignore = request;

    response->set_is_enabled(m_status_request_handler());
    spdlog::debug("{} status sent", (response->is_enabled() ? "Enabled" : "Disabled"));

    return {}; // OK
}

Server::Server(const std::string & host)
{
    ::grpc::ServerBuilder builder;
    builder.AddListeningPort(host, ::grpc::InsecureServerCredentials());
    builder.RegisterService(&m_service);
    m_server = builder.BuildAndStart();
}

Server::~Server()
{
    shutdown();
}

void Server::wait()
{
    assert(m_server && "LightProxyServer wait: server is nullptr");
    m_server->Wait();
}

void Server::shutdown()
{
    assert(m_server && "LightProxyServer shutdown: server is nullptr");
    m_server->Shutdown();
}

void Server::onConfigUpdate(
        open_greenery::dataflow::common::AsyncReceive<
                open_greenery::dataflow::relay::Config> handler)
{
    m_service.m_config_update_handler = std::move(handler);
}

void Server::onManualControl(
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Control> handler)
{
    m_service.m_manual_control_handler = std::move(handler);
}

void Server::onModeUpdate(
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Mode> handler)
{
    m_service.m_mode_update_handler = std::move(handler);
}

void Server::onStatusRequest(open_greenery::dataflow::common::AsyncProvide<bool> handler)
{
    m_service.m_status_request_handler = std::move(handler);
}

void Server::onUpdate(
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Config> receive)
{
    onConfigUpdate(std::move(receive));
}

void Server::onUpdate(
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Control> receive)
{
    onManualControl(std::move(receive));
}

void Server::onUpdate(
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Mode> receive)
{
    onModeUpdate(std::move(receive));
}

void Server::onRequest(open_greenery::dataflow::common::AsyncProvide<bool> provide)
{
    onStatusRequest(std::move(provide));
}

}
