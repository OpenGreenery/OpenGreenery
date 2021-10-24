#include "open_greenery/rpc/light/LightProxyClient.hpp"

#include <utility>
#include <grpcpp/create_channel.h>
#include <grpcpp/channel.h>
#include "light.grpc.pb.h"
#include <spdlog/spdlog.h>

namespace open_greenery::rpc::light
{

LightProxyClient::LightProxyClient(const std::string & host)
        :m_channel(grpc::CreateChannel(host, grpc::InsecureChannelCredentials())),
        m_stub(LightProxy::NewStub(m_channel)) {}


LightProxyClient::StubUser::StubUser(std::shared_ptr<LightProxy::Stub> stub)
    :m_stub(std::move(stub)) {}


LightProxyClient::ConfigProvider::ConfigProvider(std::shared_ptr<LightProxy::Stub> stub)
    :StubUser(std::move(stub)) {}

std::optional<open_greenery::dataflow::light::LightConfigRecord> LightProxyClient::ConfigProvider::get()
{
    grpc::ClientContext context;
    const google::protobuf::Empty request;
    ConfigResponse response;

    const auto status = m_stub->GetConfig(&context, request, &response);
    if (!status.ok())
    {
        spdlog::trace("Unsuccessful config request");
        return {};
    }

    if (response.has_day_start() != response.has_day_end())
    {
        spdlog::warn("Only one time specified in config");
        return {};
    }

    if (response.has_day_start() && response.has_day_end())
    {
        std::optional<open_greenery::dataflow::light::LightConfigRecord> received_config {
            {QTime::fromMSecsSinceStartOfDay(response.day_start()),
             QTime::fromMSecsSinceStartOfDay(response.day_end())}
        };
        auto TimeStr = [](const QTime & t){return t.toString("hh:mm:ss").toStdString();};
        spdlog::debug("Received config: start={}, end={}",
                     TimeStr(received_config->day_start),
                     TimeStr(received_config->day_end));
        return received_config;
    }
    return {};
}

LightProxyClient::ManualControlProvider::ManualControlProvider(std::shared_ptr<LightProxy::Stub> stub)
    :StubUser(std::move(stub)) {}

std::optional<open_greenery::dataflow::light::Control> LightProxyClient::ManualControlProvider::get()
{
    grpc::ClientContext context;
    const google::protobuf::Empty request;
    ManualControlResponse response;

    const auto status = m_stub->GetManualControl(&context, request, &response);
    if (!status.ok())
    {
        spdlog::trace("Unsuccessful manual control request");
        return {};
    }

    if (response.has_control())
    {
        const auto control = response.control();
        switch (control)
        {
            case ManualControlResponse::CONTROL_ENABLE:
                spdlog::debug("Received manual enable command");
                return {open_greenery::dataflow::light::Control::ENABLE};
            case ManualControlResponse::CONTROL_DISABLE:
                spdlog::debug("Received manual disable command");
                return {open_greenery::dataflow::light::Control::DISABLE};
            case ManualControlResponse::CONTROL_TOGGLE:
                spdlog::debug("Received manual toggle command");
                return {open_greenery::dataflow::light::Control::TOGGLE};
            default:
                assert(false && "Unknown control type");
                return {};
        }
    }
    return {};
}

LightProxyClient::ModeProvider::ModeProvider(std::shared_ptr<LightProxy::Stub> stub)
    :StubUser(std::move(stub)) {}

std::optional<open_greenery::dataflow::light::Mode> LightProxyClient::ModeProvider::get()
{
    grpc::ClientContext context;
    const google::protobuf::Empty request;
    ModeResponse response;

    const auto status = m_stub->GetMode(&context, request, &response);
    if (!status.ok())
    {
        spdlog::trace("Unsuccessful mode request");
        return {};
    }

    if (response.has_mode())
    {
        switch (response.mode())
        {
            case ModeResponse::MODE_AUTO:
                spdlog::debug("Received auto mode setting");
                return {open_greenery::dataflow::light::Mode::AUTO};
            case ModeResponse::MODE_MANUAL:
                spdlog::debug("Received manual mode setting");
                return {open_greenery::dataflow::light::Mode::MANUAL};
            default:
                assert(false && "Unknown Mode type");
                return {};
        }
    }
    return {};
}


LightProxyClient::StatusReceiver::StatusReceiver(std::shared_ptr<LightProxy::Stub> stub)
    :StubUser(std::move(stub)) {}

void LightProxyClient::StatusReceiver::set(bool _is_enabled)
{
    grpc::ClientContext context;
    StatusReport request;
    google::protobuf::Empty response;

    request.set_is_enabled(_is_enabled);

    const auto status = m_stub->SetStatus(&context, request, &response);
    if (!status.ok())
    {
        spdlog::trace("Unsuccessful status sending");
        return;
    }
    spdlog::debug("{} status sent", (_is_enabled ? "enabled" : "disabled"));
}

std::shared_ptr<LightProxyClient::ConfigProvider> LightProxyClient::getConfigProvider()
{
    if (!m_config_provider)
    {
        m_config_provider = std::make_shared<ConfigProvider>(m_stub);
    }
    return m_config_provider;
}

std::shared_ptr<LightProxyClient::ManualControlProvider> LightProxyClient::getManualControlProvider()
{
    if (!m_manual_control_provider)
    {
        m_manual_control_provider = std::make_shared<ManualControlProvider>(m_stub);
    }
    return m_manual_control_provider;
}

std::shared_ptr<LightProxyClient::ModeProvider> LightProxyClient::getModeProvider()
{
    if (!m_mode_provider)
    {
        m_mode_provider = std::make_shared<ModeProvider>(m_stub);
    }
    return m_mode_provider;
}

std::shared_ptr<LightProxyClient::StatusReceiver> LightProxyClient::getStatusReceiver()
{
    if (!m_status_receiver)
    {
        m_status_receiver = std::make_shared<StatusReceiver>(m_stub);
    }
    return m_status_receiver;
}

}
