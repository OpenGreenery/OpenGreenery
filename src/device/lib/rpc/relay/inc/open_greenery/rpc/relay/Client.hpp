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
        public open_greenery::dataflow::relay::IModeReceiver,
        public open_greenery::dataflow::relay::IStatusOptionalProvider
{
public:
    explicit Client(const std::string & host);

    // IConfigReceiver
    void set(open_greenery::dataflow::relay::Config record) override;

    // IManualControlReceiver
    void set(open_greenery::dataflow::relay::Control control) override;

    // IModeReceiver
    void set(open_greenery::dataflow::relay::Mode mode) override;

    // IStatusProvider
    std::optional<bool> get() override;

private:
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<Relay::Stub> m_stub;
};

}

#endif //LIGHT_PROXY_CLIENT_HPP
