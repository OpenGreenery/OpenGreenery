#include "open_greenery/rpc/relay/Client.hpp"

#include <utility>
#include <grpcpp/create_channel.h>
#include <grpcpp/channel.h>
#include <spdlog/spdlog.h>

namespace open_greenery::rpc::relay
{

Client::Client(const std::string & host)
        :m_channel(grpc::CreateChannel(host, grpc::InsecureChannelCredentials())),
        m_stub(Relay::NewStub(m_channel)) {}

void Client::set(open_greenery::dataflow::relay::Config record)
{
    grpc::ClientContext context;
    Config request;
    google::protobuf::Empty response;

    const auto day_start = record.day_start.msecsSinceStartOfDay();
    request.set_day_start(day_start);
    const auto day_end = record.day_end.msecsSinceStartOfDay();
    request.set_day_end(day_end);

    auto TimeStr = [](const QTime & t){return t.toString("hh:mm:ss").toStdString();};
    spdlog::debug("Send relay config: start={}, end={}",
                  TimeStr(record.day_start),
                  TimeStr(record.day_end));

    auto status = m_stub->SetConfig(&context, request, &response);
    if (!status.ok())
    {
        spdlog::warn("Unsuccessful Relay::SetConfig request: {} {}",
                     status.error_code(),
                     status.error_message());
    }
}

void Client::set(open_greenery::dataflow::relay::Control control)
{
    grpc::ClientContext context;
    ManualControlRequest request;
    google::protobuf::Empty response;

    switch (control)
    {
        case open_greenery::dataflow::relay::Control::ENABLE:
            spdlog::debug("Send manual enable");
            request.set_control(ManualControlRequest::CONTROL_ENABLE);
            break;
        case open_greenery::dataflow::relay::Control::DISABLE:
            spdlog::debug("Send manual disable");
            request.set_control(ManualControlRequest::CONTROL_DISABLE);
            break;
        case open_greenery::dataflow::relay::Control::TOGGLE:
            spdlog::debug("Send manual toggle");
            request.set_control(ManualControlRequest::CONTROL_TOGGLE);
            break;
        default:
            assert(false && "Unknown light::Control type");
    }

    auto status = m_stub->ManualControl(&context, request, &response);
    if (!status.ok())
    {
        spdlog::warn("Unsuccessful Relay::ManualControl request: {} {}",
                     status.error_code(),
                     status.error_message());
    }
}

void Client::set(open_greenery::dataflow::relay::Mode mode)
{
    grpc::ClientContext context;
    ModeSetting request;
    google::protobuf::Empty response;

    switch (mode)
    {
        case open_greenery::dataflow::relay::Mode::MANUAL:
            spdlog::debug("Send mode setting: MANUAL");
            request.set_mode(ModeSetting::MODE_MANUAL);
            break;
        case open_greenery::dataflow::relay::Mode::AUTO:
            spdlog::debug("Send mode setting: AUTO");
            request.set_mode(ModeSetting::MODE_AUTO);
            break;
        default:
            assert(false && "Unknown relay::Mode type");
    }

    auto status = m_stub->SetMode(&context, request, &response);
    if (!status.ok())
    {
        spdlog::warn("Unsuccessful Relay::SetMode request: {} {}",
                     status.error_code(),
                     status.error_message());
    }
}

std::optional<bool> Client::get()
{
    grpc::ClientContext context;
    google::protobuf::Empty request;
    Status response;

    auto status = m_stub->GetStatus(&context, request, &response);
    if (!status.ok())
    {
        spdlog::warn("Unsuccessful Relay::GetStatus request: {} {}",
                     status.error_code(),
                     status.error_message());
        return {};
    }

    return response.is_enabled();
}

}
