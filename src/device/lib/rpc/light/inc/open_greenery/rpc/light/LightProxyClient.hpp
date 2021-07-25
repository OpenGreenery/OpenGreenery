#ifndef LIGHT_PROXY_CLIENT_HPP
#define LIGHT_PROXY_CLIENT_HPP

#include <open_greenery/dataflow/light/IConfigProvider.hpp>
#include <open_greenery/dataflow/light/IManualControlProvider.hpp>
#include <open_greenery/dataflow/light/IModeProvider.hpp>
#include <open_greenery/dataflow/light/IStatusReceiver.hpp>
#include "light.grpc.pb.h"

namespace open_greenery::rpc::light
{

class LightProxyClient :
        public open_greenery::dataflow::light::IConfigProvider,
        public open_greenery::dataflow::light::IManualControlProvider,
        public open_greenery::dataflow::light::IModeProvider,
        public open_greenery::dataflow::light::IStatusReceiver
{
public:
//    // IConfigProvider
//    std::optional<open_greenery::dataflow::light::LightConfigRecord> get() override;
//
//    // IManualControlProvider
//    std::optional<open_greenery::dataflow::light::Control> get() override;
//
//    // IModeProvider
//    std::optional<open_greenery::dataflow::light::Mode> get() override;
//
//    // IStatusReceiver
//    void set(bool _is_enabled) override;
};

}

#endif //LIGHT_PROXY_CLIENT_HPP
