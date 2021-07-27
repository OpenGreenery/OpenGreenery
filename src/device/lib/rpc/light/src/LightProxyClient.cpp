#include "open_greenery/rpc/light/LightProxyClient.hpp"

#include <utility>

namespace open_greenery::rpc::light
{

LightProxyClient::LightProxyClient(std::shared_ptr<::grpc::Channel> channel)
        :m_stub(LightProxy::NewStub(channel)) {}


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
        std::cerr << "Invalid light service config request" << std::endl;
        return {};
    }

    if (response.has_day_start() && response.has_day_end())
    {
        return {{QTime::fromMSecsSinceStartOfDay(response.day_start()),
                QTime::fromMSecsSinceStartOfDay(response.day_end())}};
    }
    else
    {
        return {};
    }
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
        std::cerr << "Invalid light service manual control request" << std::endl;
        return {};
    }

    if (response.has_control())
    {
        const auto control = response.control();
        switch (control)
        {
            case ManualControlResponse::CONTROL_ENABLE:
                return {open_greenery::dataflow::light::Control::ENABLE};
            case ManualControlResponse::CONTROL_DISABLE:
                return {open_greenery::dataflow::light::Control::DISABLE};
            case ManualControlResponse::CONTROL_TOGGLE:
                return {open_greenery::dataflow::light::Control::TOGGLE};
            default:
                assert(false && "Unknown light::control type");
        }
    }
    else
    {
        return {};
    }
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
        std::cerr << "Invalid light service mode request" << std::endl;
        return {};
    }

    if (response.has_mode())
    {
        switch (response.mode())
        {
            case ModeResponse::MODE_AUTO:
                return {open_greenery::dataflow::light::Mode::AUTO};
            case ModeResponse::MODE_MANUAL:
                return {open_greenery::dataflow::light::Mode::MANUAL};
            default:
                assert(false && "Unknown light::Mode type");
        }
    }
    else
    {
        return {};
    }
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
        std::cerr << "Invalid light service status report" << std::endl;
        return;
    }
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
