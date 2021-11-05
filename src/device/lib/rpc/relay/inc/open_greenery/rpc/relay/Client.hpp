#ifndef LIGHT_PROXY_CLIENT_HPP
#define LIGHT_PROXY_CLIENT_HPP

#include <memory>
#include <open_greenery/dataflow/light/Participants.hpp>
#include "relay.grpc.pb.h"

namespace open_greenery::rpc::relay
{

class Client :
        public open_greenery::dataflow::light::IConfigReceiver,
        public open_greenery::dataflow::light::IManualControlReceiver,
        public open_greenery::dataflow::light::IModeReceiver,
        public open_greenery::dataflow::light::IStatusOptionalProvider
{
public:
    explicit Client(const std::string & host);

    // IConfigReceiver
    void set(open_greenery::dataflow::light::LightConfigRecord record) override;

    // IManualControlReceiver
    void set(open_greenery::dataflow::light::Control control) override;

    // IModeReceiver
    void set(open_greenery::dataflow::light::Mode mode) override;

    // IStatusProvider
    std::optional<bool> get() override;

private:
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<Relay::Stub> m_stub;
};

}

#endif //LIGHT_PROXY_CLIENT_HPP
