#ifndef LIGHT_PROXY_CLIENT_HPP
#define LIGHT_PROXY_CLIENT_HPP

#include <memory>
#include <open_greenery/dataflow/relay/Participants.hpp>
#include "relay.grpc.pb.h"

namespace open_greenery::rpc::relay
{

class Client :
        public open_greenery::dataflow::relay::IConfigReceiver,
        public open_greenery::dataflow::relay::IManualControlReceiver,
        public open_greenery::dataflow::relay::IModeReceiver
{
public:
    explicit Client(const std::string & host);

    // IConfigReceiver
    void set(open_greenery::dataflow::relay::Config record) override;

    // IManualControlReceiver
    void set(open_greenery::dataflow::relay::Control control) override;

    // IModeReceiver
    void set(open_greenery::dataflow::relay::Mode mode) override;

    class RelayStatusOptionalProvider : public open_greenery::dataflow::relay::IRelayStatusOptionalProvider
    {
    public:
        RelayStatusOptionalProvider(std::shared_ptr<Relay::Stub> stub);

        std::optional<bool> get() override;

    private:
        std::shared_ptr<Relay::Stub> m_stub;
    };

    class ServiceStatusOptionalProvider : public open_greenery::dataflow::relay::IServiceStatusOptionalProvider
    {
    public:
        ServiceStatusOptionalProvider(std::shared_ptr<Relay::Stub> stub);

        std::optional<open_greenery::dataflow::relay::ServiceStatus> get() override;

    private:
        std::shared_ptr<Relay::Stub> m_stub;
    };

    std::unique_ptr<RelayStatusOptionalProvider> getRelayStatusOptionalProvider() const;

    std::unique_ptr<ServiceStatusOptionalProvider> getServiceStatusOptionalProvider() const;
private:
    std::shared_ptr<grpc::Channel> m_channel;
    std::shared_ptr<Relay::Stub> m_stub;
};

}

#endif //LIGHT_PROXY_CLIENT_HPP
