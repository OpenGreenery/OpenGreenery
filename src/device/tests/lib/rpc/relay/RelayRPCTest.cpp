#include <gtest/gtest.h>
#include <memory>

#include <open_greenery/rpc/relay/Client.hpp>
#include <open_greenery/rpc/relay/Server.hpp>

namespace ogrpcr = open_greenery::rpc::relay;
namespace ogdfl = open_greenery::dataflow::relay;

namespace open_greenery::tests::lib::rpc
{

class RelayRPCTest : public ::testing::Test
{
protected:

    void SetUp() override
    {
        constexpr static char HOST [] {"127.0.0.1:8081"};
        client = std::make_shared<ogrpcr::Client>(HOST);
        server = std::make_shared<ogrpcr::Server>(HOST);
        control_provider = server;
        control_receiver = client;
        config_provider = server;
        config_receiver = client;
        mode_provider = server;
        mode_receiver = client;
        relay_status_provider = client->getRelayStatusOptionalProvider();
        relay_status_receiver = server;
        service_status_provider = client->getServiceStatusOptionalProvider();
        service_status_receiver = server;
    }

    std::shared_ptr<ogrpcr::Client> client;
    std::shared_ptr<ogrpcr::Server> server;

    std::shared_ptr<ogdfl::IAsyncConfigProvider> config_provider;
    std::shared_ptr<ogdfl::IConfigReceiver> config_receiver;
    std::shared_ptr<ogdfl::IAsyncManualControlProvider> control_provider;
    std::shared_ptr<ogdfl::IManualControlReceiver> control_receiver;
    std::shared_ptr<ogdfl::IAsyncModeProvider> mode_provider;
    std::shared_ptr<ogdfl::IModeReceiver> mode_receiver;
    std::unique_ptr<ogdfl::IRelayStatusOptionalProvider> relay_status_provider;
    std::shared_ptr<ogdfl::IAsyncRelayStatusReceiver> relay_status_receiver;
    std::unique_ptr<ogdfl::IServiceStatusOptionalProvider> service_status_provider;
    std::shared_ptr<ogdfl::IAsyncServiceStatusReceiver> service_status_receiver;
};

TEST_F(RelayRPCTest, ConfigTransmition)
{
    const ogdfl::Config expected_config{QTime(0, 0),
                                        QTime(23, 59, 59, 999)};

    config_provider->onUpdate([expected_config](ogdfl::Config actual_config) {
        EXPECT_EQ(expected_config.day_start, actual_config.day_start);
        EXPECT_EQ(expected_config.day_end, actual_config.day_end);
    });
    config_receiver->set(expected_config);
}

TEST_F(RelayRPCTest, ManualControlTransmition)
{
    for (auto expected_ctl : {ogdfl::Control::ENABLE,
                              ogdfl::Control::DISABLE,
                              ogdfl::Control::TOGGLE})
    {
        control_provider->onUpdate([expected_ctl](ogdfl::Control actual_ctl) {
            EXPECT_EQ(expected_ctl, actual_ctl);
        });
        control_receiver->set(expected_ctl);
    }
}

TEST_F(RelayRPCTest, ModeTransmition)
{
    for (auto expected_mode : {ogdfl::Mode::AUTO,
                               ogdfl::Mode::MANUAL})
    {
        mode_provider->onUpdate([expected_mode](ogdfl::Mode actual_mode) {
            EXPECT_EQ(expected_mode, actual_mode);
        });
        mode_receiver->set(expected_mode);
    }
}

TEST_F(RelayRPCTest, RelayStatusTransmition)
{
    for (auto expected_status : {true, false})
    {
        relay_status_receiver->onRequest([expected_status] {
            return expected_status;
        });
        const auto actual_status = relay_status_provider->get();
        ASSERT_TRUE(actual_status.has_value());
        EXPECT_EQ(expected_status, *actual_status);
    }
}

TEST_F(RelayRPCTest, ServiceStatusTransmition)
{
    const ogdfl::Config expected_config{QTime(0, 0),
                                        QTime(23, 59, 59, 999)};
    for (auto expected_mode : {ogdfl::Mode::AUTO,
                               ogdfl::Mode::MANUAL})
    {
        for (auto expected_relay_status : {true, false})
        {
            service_status_receiver->onRequest([expected_mode, expected_relay_status, expected_config] {
                ogdfl::ServiceStatus service_status;
                service_status.mode = expected_mode;
                service_status.relay_enabled = expected_relay_status;
                service_status.config = expected_config;
                return service_status;
            });

            const auto actual_service_status = service_status_provider->get();
            ASSERT_TRUE(actual_service_status.has_value());
            EXPECT_EQ(expected_mode, actual_service_status->mode);
            EXPECT_EQ(expected_relay_status, actual_service_status->relay_enabled);
            EXPECT_EQ(expected_config.day_start, actual_service_status->config.day_start);
            EXPECT_EQ(expected_config.day_end, actual_service_status->config.day_end);
        }
    }
}

}
