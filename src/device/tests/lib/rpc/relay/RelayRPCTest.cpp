#include <gtest/gtest.h>
#include <memory>

#include <open_greenery/rpc/light/LightProxyClient.hpp>
#include <open_greenery/rpc/light/LightProxyServer.hpp>

namespace ogrpcl = open_greenery::rpc::light;
namespace ogdfl = open_greenery::dataflow::light;

namespace open_greenery::tests::lib::rpc
{

class RelayRPCTest : public ::testing::Test
{
protected:

    void SetUp() override
    {
        constexpr static char HOST [] {"127.0.0.1:8081"};
        client = std::make_shared<ogrpcl::LightProxyClient>(HOST);
        server = std::make_shared<ogrpcl::LightProxyServer>(HOST);
        control_provider = client->getManualControlProvider();
        control_receiver = server;
        config_provider = client->getConfigProvider();
        config_receiver = server;
        mode_provider = client->getModeProvider();
        mode_receiver = server;
        status_provider = server;
        status_receiver = client->getStatusReceiver();
    }

    std::shared_ptr<ogrpcl::LightProxyClient> client;
    std::shared_ptr<ogrpcl::LightProxyServer> server;

    std::shared_ptr<ogdfl::IConfigProvider> config_provider;
    std::shared_ptr<ogdfl::IConfigReceiver> config_receiver;
    std::shared_ptr<ogdfl::IManualControlProvider> control_provider;
    std::shared_ptr<ogdfl::IManualControlReceiver> control_receiver;
    std::shared_ptr<ogdfl::IModeProvider> mode_provider;
    std::shared_ptr<ogdfl::IModeReceiver> mode_receiver;
    std::shared_ptr<ogdfl::IStatusProvider> status_provider;
    std::shared_ptr<ogdfl::IStatusReceiver> status_receiver;
};

TEST_F(RelayRPCTest, ConfigTransmition)
{
    const ogdfl::LightConfigRecord expected_config {QTime(0, 0),
                                                    QTime(23, 59, 59, 999)};
    config_receiver->set(expected_config);
    const auto actual_config = config_provider->get();
    ASSERT_TRUE(actual_config.has_value());
    EXPECT_EQ(expected_config.day_start, actual_config->day_start);
    EXPECT_EQ(expected_config.day_end, actual_config->day_end);
}

TEST_F(RelayRPCTest, SingleConfigTransmition)
{
    EXPECT_FALSE(config_provider->get().has_value());

    config_receiver->set({QTime(), QTime()});
    EXPECT_TRUE(config_provider->get().has_value());

    // Second read shall return empty config
    EXPECT_FALSE(config_provider->get().has_value());
}

TEST_F(RelayRPCTest, ManualControlTransmition)
{
    for (auto expected_config : {ogdfl::Control::ENABLE,
                                 ogdfl::Control::DISABLE,
                                 ogdfl::Control::TOGGLE})
    {
        control_receiver->set(expected_config);
        auto actual_config = control_provider->get();
        ASSERT_TRUE(actual_config.has_value());
        EXPECT_EQ(expected_config, actual_config.value());
    }
}

TEST_F(RelayRPCTest, SingleManualControlTransmition)
{
    EXPECT_FALSE(control_provider->get().has_value());

    control_receiver->set(ogdfl::Control::ENABLE);
    EXPECT_TRUE(control_provider->get().has_value());

    // Second read shall return empty control
    EXPECT_FALSE(control_provider->get().has_value());
}

TEST_F(RelayRPCTest, ModeTransmition)
{
    for (auto expected_mode : {ogdfl::Mode::AUTO,
                               ogdfl::Mode::MANUAL})
    {
        mode_receiver->set(expected_mode);
        auto actual_mode = mode_provider->get();
        ASSERT_TRUE(actual_mode.has_value());
        EXPECT_EQ(expected_mode, actual_mode.value());
    }
}

TEST_F(RelayRPCTest, SingleModeTransmition)
{
    EXPECT_FALSE(mode_provider->get().has_value());

    mode_receiver->set(ogdfl::Mode::AUTO);
    EXPECT_TRUE(mode_provider->get().has_value());

    // Second read shall return empty mode
    EXPECT_FALSE(mode_provider->get().has_value());
}

TEST_F(RelayRPCTest, StatusTransmition)
{
    for (auto expected_status : {true, false})
    {
        status_receiver->set(expected_status);
        auto actual_status = status_provider->get();
        ASSERT_TRUE(actual_status.has_value());
        EXPECT_EQ(expected_status, actual_status.value());
    }
}

TEST_F(RelayRPCTest, SingleStatusTransmition)
{
    EXPECT_FALSE(status_provider->get().has_value());

    status_receiver->set(true);
    EXPECT_TRUE(status_provider->get().has_value());

    // Second read shall return empty mode
    EXPECT_FALSE(status_provider->get().has_value());
}

}
