#include <gtest/gtest.h>
#include <open_greenery/rpc/relay/Client.hpp>
#include <open_greenery/rpc/relay/Server.hpp>
#include <open_greenery/light/LightController.hpp>
#include <open_greenery/mock/relay/StatefulRelayMock.hpp>
#include <open_greenery/mock/dataflow/time/CurrentTimeProviderMock.hpp>

using namespace std::chrono_literals;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::ReturnPointee;
using ::testing::InSequence;

namespace ogdfl = open_greenery::dataflow::light;

namespace open_greenery::tests::integration::rpc_light
{

class LightRpcTest : public ::testing::Test
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
        proxy_status_reader = rpc_client;

        rpc_server = std::make_shared<open_greenery::rpc::relay::Server>(RPC_HOST);
        auto config_provider = rpc_server;
        auto control_provider = rpc_server;
        auto mode_provider = rpc_server;
        auto status_receiver = rpc_server;

        light_controller.emplace(relay,
                                 config_provider,
                                 time_provider,
                                 control_provider,
                                 mode_provider,
                                 status_receiver);

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
    std::optional<open_greenery::light::LightController> light_controller;
    std::optional<open_greenery::tools::FinishFuture> controller_finish;

    std::shared_ptr<ogdfl::IConfigReceiver> proxy_config_writer;
    std::shared_ptr<ogdfl::IManualControlReceiver> proxy_control_writer;
    std::shared_ptr<ogdfl::IModeReceiver> proxy_mode_writer;
    std::shared_ptr<ogdfl::IStatusOptionalProvider> proxy_status_reader;
};

TEST_F(LightRpcTest, ManualEnable)
{
    EXPECT_CALL(*relay, enable())
        .Times(1);  // One enable for one command

    proxy_control_writer->set(ogdfl::Control::ENABLE);
    waitForHandling();
    const auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());
}

TEST_F(LightRpcTest, ManualDisable)
{
    EXPECT_CALL(*relay, disable())
            .Times(1);  // One disable for one command

    proxy_control_writer->set(ogdfl::Control::DISABLE);
    waitForHandling();
    const auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(LightRpcTest, ManualToggle)
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
    auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    proxy_control_writer->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    proxy_control_writer->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());
}

TEST_F(LightRpcTest, HandleManualInAuto)
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
    auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    proxy_control_writer->set(ogdfl::Control::DISABLE);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    proxy_control_writer->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());
}

TEST_F(LightRpcTest, AutoEnable)
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
    auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    ogdfl::LightConfigRecord cfg {
        QTime(0, 5),  // 00:05
        QTime(23, 59)  // 23:59
    };
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value()); // still disabled

    current_time = QTime(0, 5);  // day start
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value()); // still enabled
}

TEST_F(LightRpcTest, AutoDisable)
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
    auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    ogdfl::LightConfigRecord cfg {
        QTime(23, 59),  // 23:59
        QTime(0, 5)  // 00:05
    };
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value()); // still enabled

    current_time = QTime(0, 5);  // day start
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value()); // disabled
}

TEST_F(LightRpcTest, DontHandleAutoInManual)
{
    ogdfl::LightConfigRecord cfg {
        QTime(0, 1),
        QTime(0, 2)
    };
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::MANUAL);

    current_time = QTime(0, 1);
    waitForHandling();
    auto status = proxy_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    current_time = QTime(0, 2);
    waitForHandling();
    status = proxy_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(LightRpcTest, ManualByDefault)
{
    ogdfl::LightConfigRecord cfg {
        QTime(0, 1),
        QTime(0, 2)
    };
    proxy_config_writer->set(cfg); // 00:01, 00:02

    current_time = QTime(0, 1);
    waitForHandling();
    auto status = proxy_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    current_time = QTime(0, 2);
    waitForHandling();
    status = proxy_status_reader->get();
    EXPECT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(LightRpcTest, ChangeConfig)
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
    ogdfl::LightConfigRecord first_cfg {QTime(0, 1), QTime(0, 2)}; // 00:01, 00:02
    proxy_mode_writer->set(ogdfl::Mode::AUTO);
    proxy_config_writer->set(first_cfg);

    current_time = QTime(0, 1);
    waitForHandling();
    auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    current_time = QTime(0, 2);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());

    // Second config
    ogdfl::LightConfigRecord second_cfg {QTime(20, 1), QTime(20, 2)}; // 20:01, 20:02
    proxy_config_writer->set(second_cfg);

    current_time = QTime(20, 1);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    current_time = QTime(20, 2);
    waitForHandling();
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

TEST_F(LightRpcTest, AutoControlDuplication)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
    }

    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};  // 00:01, 00:02
    proxy_config_writer->set(cfg);
    proxy_mode_writer->set(ogdfl::Mode::AUTO);

    current_time = QTime(0, 1);
    waitForHandling(DEFAULT_HANDLING_TIME*5);
    auto status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_TRUE(status.value());

    current_time = QTime(0, 2);
    waitForHandling(DEFAULT_HANDLING_TIME*5);
    status = proxy_status_reader->get();
    ASSERT_TRUE(status.has_value());
    EXPECT_FALSE(status.value());
}

}
