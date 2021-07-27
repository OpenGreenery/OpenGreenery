#ifndef LIGHT_PROXY_CLIENT_HPP
#define LIGHT_PROXY_CLIENT_HPP

#include <memory>
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <open_greenery/dataflow/light/IConfigProvider.hpp>
#include <open_greenery/dataflow/light/IManualControlProvider.hpp>
#include <open_greenery/dataflow/light/IModeProvider.hpp>
#include <open_greenery/dataflow/light/IStatusReceiver.hpp>
#include "light.grpc.pb.h"

namespace open_greenery::rpc::light
{

class LightProxyClient
{
public:
    explicit LightProxyClient(std::shared_ptr<::grpc::Channel> channel);

    class ConfigProvider;
    class ManualControlProvider;
    class ModeProvider;
    class StatusReceiver;

    std::shared_ptr<ConfigProvider> getConfigProvider();
    std::shared_ptr<ManualControlProvider> getManualControlProvider();
    std::shared_ptr<ModeProvider> getModeProvider();
    std::shared_ptr<StatusReceiver> getStatusReceiver();

protected:
    class StubUser;

private:
    std::shared_ptr<LightProxy::Stub> m_stub;

    std::shared_ptr<ConfigProvider> m_config_provider;
    std::shared_ptr<ManualControlProvider> m_manual_control_provider;
    std::shared_ptr<ModeProvider> m_mode_provider;
    std::shared_ptr<StatusReceiver> m_status_receiver;
};

class LightProxyClient::StubUser
{
protected:
    explicit StubUser(std::shared_ptr<LightProxy::Stub> stub);
    std::shared_ptr<LightProxy::Stub> m_stub;
};

class LightProxyClient::ConfigProvider:
        protected StubUser,
        public open_greenery::dataflow::light::IConfigProvider
{
public:
    explicit ConfigProvider(std::shared_ptr<LightProxy::Stub> stub);

    // IConfigProvider
    std::optional<open_greenery::dataflow::light::LightConfigRecord> get() override;
};

class LightProxyClient::ManualControlProvider:
        protected StubUser,
        public open_greenery::dataflow::light::IManualControlProvider
{
public:
    explicit ManualControlProvider(std::shared_ptr<LightProxy::Stub> stub);

    // IManualControlProvider
    std::optional<open_greenery::dataflow::light::Control> get() override;
};

class LightProxyClient::ModeProvider:
        protected StubUser,
        public open_greenery::dataflow::light::IModeProvider
{
public:
    explicit ModeProvider(std::shared_ptr<LightProxy::Stub> stub);

    // IModeProvider
    std::optional<open_greenery::dataflow::light::Mode> get() override;
};

class LightProxyClient::StatusReceiver:
        protected StubUser,
        public open_greenery::dataflow::light::IStatusReceiver
{
public:
    explicit StatusReceiver(std::shared_ptr<LightProxy::Stub> stub);

    // IStatusReceiver
    void set(bool _is_enabled) override;
};

}

#endif //LIGHT_PROXY_CLIENT_HPP
