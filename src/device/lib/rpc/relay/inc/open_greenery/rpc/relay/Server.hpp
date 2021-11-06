#ifndef SERVER_HPP
#define SERVER_HPP

#include <optional>
#include <grpcpp/server.h>
#include <open_greenery/dataflow/light/Participants.hpp>
#include "relay.grpc.pb.h"

namespace open_greenery::rpc::relay
{

class Server :
        public open_greenery::dataflow::light::IAsyncConfigProvider,
        public open_greenery::dataflow::light::IAsyncManualControlProvider,
        public open_greenery::dataflow::light::IAsyncModeProvider,
        public open_greenery::dataflow::light::IAsyncStatusReceiver
{
public:
    explicit Server(const std::string & host);

    ~Server() override;

    void wait();

    void shutdown();

    void onConfigUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::light::LightConfigRecord> handler);

    void onUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::light::LightConfigRecord> receive) override;

    void onManualControl(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::light::Control> handler);

    void onUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::light::Control> receive) override;

    void onModeUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::light::Mode> handler);

    void onUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::light::Mode> receive) override;

    void onStatusRequest(open_greenery::dataflow::common::AsyncProvide<bool> handler);

    void onRequest(open_greenery::dataflow::common::AsyncProvide<bool> provide) override;

private:
    class Service : public Relay::Service
    {
    public:
        // Relay::Service gRPC
        grpc::Status SetConfig(grpc::ServerContext * context,
                               const open_greenery::rpc::relay::Config * request,
                               google::protobuf::Empty * response) override;

        grpc::Status ManualControl(grpc::ServerContext * context,
                                   const open_greenery::rpc::relay::ManualControlRequest * request,
                                   google::protobuf::Empty * response) override;

        grpc::Status SetMode(grpc::ServerContext * context,
                             const open_greenery::rpc::relay::ModeSetting * request,
                             google::protobuf::Empty * response) override;

        grpc::Status GetStatus(grpc::ServerContext * context,
                               const google::protobuf::Empty * request,
                               open_greenery::rpc::relay::Status * response) override;

        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::light::LightConfigRecord> m_config_update_handler;
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::light::Control> m_manual_control_handler;
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::light::Mode> m_mode_update_handler;
        open_greenery::dataflow::common::AsyncProvide<bool> m_status_request_handler;
    };

    Service m_service;
    std::unique_ptr<grpc::Server> m_server;
};

}

#endif //SERVER_HPP
