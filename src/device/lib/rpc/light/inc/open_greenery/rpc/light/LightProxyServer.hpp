#ifndef LIGHTPROXYSERVER_HPP
#define LIGHTPROXYSERVER_HPP

#include <optional>
#include <grpcpp/server.h>
#include <open_greenery/dataflow/light/Participants.hpp>
#include "light.grpc.pb.h"

namespace open_greenery::rpc::light
{

class LightProxyServer :
        public open_greenery::dataflow::light::IConfigReceiver,
        public open_greenery::dataflow::light::IManualControlReceiver,
        public open_greenery::dataflow::light::IModeReceiver,
        public open_greenery::dataflow::light::IStatusProvider
{
public:
    explicit LightProxyServer(const std::string & host);
    ~LightProxyServer() override;

    // IConfigReceiver
    void set(open_greenery::dataflow::light::LightConfigRecord record) override;

    // IManualControlReceiver
    void set(open_greenery::dataflow::light::Control control) override;

    // IModeReceiver
    void set(open_greenery::dataflow::light::Mode mode) override;

    // IStatusProvider
    std::optional<bool> get() override;

    void wait();

    void shutdown();

private:
    class GrpcService : public LightProxy::Service
    {
    public:
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

        void set(open_greenery::dataflow::light::LightConfigRecord record);
        void set(open_greenery::dataflow::light::Control control);
        void set(open_greenery::dataflow::light::Mode mode);
        std::optional<bool> get();
    private:
        std::optional<open_greenery::dataflow::light::LightConfigRecord> m_config;
        std::optional<open_greenery::dataflow::light::Control> m_manual_control;
        std::optional<open_greenery::dataflow::light::Mode> m_mode;
        std::optional<bool> m_status;
    };

    GrpcService m_service;
    std::unique_ptr<::grpc::Server> m_server;
};

}

#endif //LIGHTPROXYSERVER_HPP
