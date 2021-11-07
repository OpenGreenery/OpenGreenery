#ifndef SERVER_HPP
#define SERVER_HPP

#include <optional>
#include <grpcpp/server.h>
#include <open_greenery/dataflow/relay/Participants.hpp>
#include "relay.grpc.pb.h"

namespace open_greenery::rpc::relay
{

class Server :
        public open_greenery::dataflow::relay::IAsyncConfigProvider,
        public open_greenery::dataflow::relay::IAsyncManualControlProvider,
        public open_greenery::dataflow::relay::IAsyncModeProvider,
        public open_greenery::dataflow::relay::IAsyncStatusReceiver
{
public:
    explicit Server(const std::string & host);

    ~Server() override;

    void wait();

    void shutdown();

    void onConfigUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::relay::Config> handler);

    void onUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::relay::Config> receive) override;

    void onManualControl(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::relay::Control> handler);

    void onUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::relay::Control> receive) override;

    void onModeUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::relay::Mode> handler);

    void onUpdate(
            open_greenery::dataflow::common::AsyncReceive
                    <open_greenery::dataflow::relay::Mode> receive) override;

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
                <open_greenery::dataflow::relay::Config> m_config_update_handler;
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Control> m_manual_control_handler;
        open_greenery::dataflow::common::AsyncReceive
                <open_greenery::dataflow::relay::Mode> m_mode_update_handler;
        open_greenery::dataflow::common::AsyncProvide<bool> m_status_request_handler;
    };

    Service m_service;
    std::unique_ptr<grpc::Server> m_server;
};

}

#endif //SERVER_HPP
