#include <gtest/gtest.h>
#include <open_greenery/rpc/relay/Client.hpp>
#include <open_greenery/rpc/relay/Server.hpp>
#include <open_greenery/relay/RelayController.hpp>
#include <open_greenery/mock/relay/StatefulRelayMock.hpp>
#include <open_greenery/mock/dataflow/time/CurrentTimeProviderMock.hpp>

using namespace std::chrono_literals;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::ReturnPointee;
using ::testing::InSequence;

namespace ogdfl = open_greenery::dataflow::relay;

namespace open_greenery::tests::integration::relay_rpc
{

class RelayRpcTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        relay = std::make_shared<open_greenery::mock::relay::StatefulRelayMock>();
        EXPECT_CALL(*relay, enabled()).Times(AnyNumber());  // Uninteresting method

        time_provider = std::make_shared<open_greenery::mock::dataflow::time::CurrentTimeProviderMock>();
        EXPECT_CALL(*time_provider, get())
                .Times(AnyNumber())
                .WillRepeatedly(ReturnPointee(&current_time));

        rpc_client = std::make_shared<open_greenery::rpc::relay::Client>(RPC_HOST);
        proxy_config_writer = rpc_client;
        proxy_control_writer = rpc_client;
        proxy_mode_writer = rpc_client;
        proxy_relay_status_reader = rpc_client->getRelayStatusOptionalProvider();
        proxy_service_status_reader = rpc_client->getServiceStatusOptionalProvider();

        rpc_server = std::make_shared<open_greenery::rpc::relay::Server>(RPC_HOST);
        auto config_provider = rpc_server;
        auto control_provider = rpc_server;
        auto mode_provider = rpc_server;
        auto relay_status_receiver = rpc_server;
        auto service_status_receiver = rpc_server;

        light_controller.emplace(relay,
                                 config_provider,
                                 time_provider,
                                 control_provider,
                                 mode_provider,
                                 relay_status_receiver,
                                 service_status_receiver);

        controller_finish = light_controller->start();
    }

    void TearDown() override
    {
        light_controller->stop();
        if (controller_finish->valid())
        {
            controller_finish->wait();
        }

        rpc_server->shutdown();
    }

    void waitForHandling(std::chrono::milliseconds duration = DEFAULT_HANDLING_TIME)
    {
        std::this_thread::sleep_for(duration);
    }

    static constexpr char RPC_HOST [] {"localhost:8085"};
    static constexpr std::chrono::milliseconds DEFAULT_HANDLING_TIME {150ms};

    std::shared_ptr<open_greenery::rpc::relay::Client> rpc_client;
    std::shared_ptr<open_greenery::rpc::relay::Server> rpc_server;

    std::shared_ptr<open_greenery::mock::relay::StatefulRelayMock> relay;
    std::shared_ptr<open_greenery::mock::dataflow::time::CurrentTimeProviderMock> time_provider;
    QTime current_time;
    std::optional<open_greenery::relay::RelayController> light_controller;
    std::optional<open_greenery::tools::FinishFuture> controller_finish;

    std::shared_ptr<ogdfl::IConfigReceiver> proxy_config_writer;
    std::shared_ptr<ogdfl::IManualControlReceiver> proxy_control_writer;
    std::shared_ptr<ogdfl::IModeReceiver> proxy_mode_writer;
    std::unique_ptr<ogdfl::IRelayStatusOptionalProvider> proxy_relay_status_reader;
    std::unique_ptr<ogdfl::IServiceStatusOptionalProvider> proxy_service_status_reader;
};

TEST_F(RelayRpcTest, ManualEnable)
{
    EXPECT_CALL(*relay, enable())
        .Times(1);  // One enable for one command

    proxy_control_writer->set(ogdfl::Control::ENABLE);
    waitForHandling();
    const auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());
}

TEST_F(RelayRpcTest, ManualDisable)
{
    EXPECT_CALL(*relay, disable())
            .Times(1);  // One disable for one command

    proxy_control_writer->set(ogdfl::Control::DISABLE);
    waitForHandling();
    const auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(RelayRpcTest, ManualToggle)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, toggle())
                .Times(2);
    }

    proxy_control_writer->set(ogdfl::Control::ENABLE);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    proxy_control_writer->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    proxy_control_writer->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());
}

TEST_F(RelayRpcTest, HandleManualInAuto)
{
    EXPECT_CALL(*time_provider, get())
        .Times(AnyNumber())
        .WillRepeatedly(Return(QTime()));  // always return 00:00

    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
        EXPECT_CALL(*relay, toggle())
                .Times(1);
    }

    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    proxy_control_writer->set(ogdfl::Control::ENABLE);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    proxy_control_writer->set(ogdfl::Control::DISABLE);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    proxy_control_writer->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());
}

TEST_F(RelayRpcTest, AutoEnable)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, disable())
                .Times(1);  // manual disable
        EXPECT_CALL(*relay, enable())
                .Times(1);  // auto enable
    }

    // previously disable manually
    proxy_control_writer->set(ogdfl::Control::DISABLE);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    ogdfl::Config cfg {
        QTime(0, 5),  // 00:05
        QTime(23, 59)  // 23:59
    };
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value()); // still disabled

    current_time = QTime(0, 5);  // day start
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value()); // still enabled
}

TEST_F(RelayRpcTest, AutoDisable)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);  // manual enable
        EXPECT_CALL(*relay, disable())
                .Times(1);  // auto disable
    }

    // previously disable manually
    proxy_control_writer->set(ogdfl::Control::ENABLE);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    ogdfl::Config cfg {
        QTime(23, 59),  // 23:59
        QTime(0, 5)  // 00:05
    };
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value()); // still enabled

    current_time = QTime(0, 5);  // day start
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value()); // disabled
}

TEST_F(RelayRpcTest, DontHandleAutoInManual)
{
    ogdfl::Config cfg {
        QTime(0, 1),
        QTime(0, 2)
    };
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::MANUAL);

    current_time = QTime(0, 1);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    current_time = QTime(0, 2);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(RelayRpcTest, ManualByDefault)
{
    ogdfl::Config cfg {
        QTime(0, 1),
        QTime(0, 2)
    };
    proxy_config_writer->set(cfg); // 00:01, 00:02

    current_time = QTime(0, 1);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    current_time = QTime(0, 2);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(RelayRpcTest, ChangeConfig)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
    }

    // First config
    ogdfl::Config first_cfg {QTime(0, 1), QTime(0, 2)}; // 00:01, 00:02
    proxy_mode_writer->set(ogdfl::Mode::AUTO);
    proxy_config_writer->set(first_cfg);

    current_time = QTime(0, 1);
    waitForHandling();
    auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    current_time = QTime(0, 2);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    // Second config
    ogdfl::Config second_cfg {QTime(20, 1), QTime(20, 2)}; // 20:01, 20:02
    proxy_config_writer->set(second_cfg);

    current_time = QTime(20, 1);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    current_time = QTime(20, 2);
    waitForHandling();
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(RelayRpcTest, AutoControlDuplication)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
    }

    ogdfl::Config cfg {QTime(0, 1), QTime(0, 2)};  // 00:01, 00:02
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    current_time = QTime(0, 1);
    waitForHandling(DEFAULT_HANDLING_TIME*5);
    auto status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    current_time = QTime(0, 2);
    waitForHandling(DEFAULT_HANDLING_TIME*5);
    status = proxy_relay_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(RelayRpcTest, ServiceStatusRequest)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
        EXPECT_CALL(*relay, toggle())
                .Times(2);
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
        EXPECT_CALL(*relay, toggle())
                .Times(2);
    }

    const ogdfl::Config expected_config{QTime(0, 0),
                                        QTime(23, 59, 59, 999)};
    for (const auto expected_mode : {ogdfl::Mode::AUTO, ogdfl::Mode::MANUAL})
    {
        for (const auto [command, expected_status] : std::array<std::tuple<ogdfl::Control, bool>, 4>{
                std::make_tuple(ogdfl::Control::ENABLE,  true),
                std::make_tuple(ogdfl::Control::DISABLE, false),
                std::make_tuple(ogdfl::Control::TOGGLE,  true),
                std::make_tuple(ogdfl::Control::TOGGLE,  false)
        })
        {
            proxy_mode_writer->set(expected_mode);
            proxy_control_writer->set(command);
            proxy_config_writer->set(expected_config);

            const auto actual_status = proxy_service_status_reader->get();
            ASSERT_TRUE(actual_status.has_value());
            EXPECT_EQ(expected_mode, actual_status->mode);
            EXPECT_EQ(expected_status, actual_status->relay_enabled);
            EXPECT_EQ(expected_config.day_start, actual_status->config.day_start);
            EXPECT_EQ(expected_config.day_end, actual_status->config.day_end);
        }
    }
}

}
