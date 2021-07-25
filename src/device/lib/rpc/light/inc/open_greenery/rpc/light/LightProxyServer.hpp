#ifndef LIGHTPROXYSERVER_HPP
#define LIGHTPROXYSERVER_HPP

#include <optional>
#include <open_greenery/dataflow/light/IConfigReceiver.hpp>
#include <open_greenery/dataflow/light/IManualControlReceiver.hpp>
#include <open_greenery/dataflow/light/IModeReceiver.hpp>
#include <open_greenery/dataflow/light/IStatusProvider.hpp>
#include "light.grpc.pb.h"

namespace open_greenery::rpc::light
{

class LightProxyServer :
        public LightProxy::Service,
        public open_greenery::dataflow::light::IConfigReceiver,
        public open_greenery::dataflow::light::IManualControlReceiver,
        public open_greenery::dataflow::light::IModeReceiver,
        public open_greenery::dataflow::light::IStatusProvider
{
    // LightProxy::Service gRPC
    ::grpc::Status GetConfig(::grpc::ServerContext * context,
                             const ::google::protobuf::Empty * request,
                             ::open_greenery::rpc::light::ConfigResponse * response) override;
    ::grpc::Status GetManualControl(::grpc::ServerContext * context,
                                    const ::google::protobuf::Empty * request,
                                    ::open_greenery::rpc::light::ManualControlResponse * response) override;
    ::grpc::Status GetMode(::grpc::ServerContext * context,
                           const ::google::protobuf::Empty * request,
                           ::open_greenery::rpc::light::ModeResponse * response) override;
    ::grpc::Status SetStatus(::grpc::ServerContext * context,
                             const ::open_greenery::rpc::light::StatusReport * request,
                             ::google::protobuf::Empty * response) override;

    // IConfigReceiver
    void set(open_greenery::dataflow::light::LightConfigRecord record) override;

    // IManualControlReceiver
    void set(open_greenery::dataflow::light::Control control) override;

    // IModeReceiver
    void set(open_greenery::dataflow::light::Mode mode) override;

    // IStatusProvider
    std::optional<bool> get() override;

private:
    std::optional<open_greenery::dataflow::light::LightConfigRecord> m_config;
    std::optional<open_greenery::dataflow::light::Control> m_manual_control;
    std::optional<open_greenery::dataflow::light::Mode> m_mode;
    std::optional<bool> m_status;
};

}

#endif //LIGHTPROXYSERVER_HPP
