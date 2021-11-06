#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include <open_greenery/light/LightController.hpp>
#include <open_greenery/dataflow/common/IAsyncReceiver.hpp>
#include <open_greenery/dataflow/common/IAsyncProvider.hpp>
#include <open_greenery/mock/dataflow/light/IAsyncConfigProviderMock.hpp>
#include <open_greenery/mock/dataflow/light/IAsyncManualControlProviderMock.hpp>
#include <open_greenery/mock/dataflow/light/IAsyncModeProviderMock.hpp>
#include <open_greenery/mock/dataflow/light/IAsyncStatusRecevierMock.hpp>
#include <open_greenery/mock/relay/StatefulRelayMock.hpp>
#include <open_greenery/mock/dataflow/time/CurrentTimeProviderMock.hpp>

namespace ogdfl = open_greenery::dataflow::light;
namespace ogl = open_greenery::light;
using namespace std::chrono_literals;
using ::testing::_;
using ::testing::SaveArg;
using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::ReturnPointee;

namespace open_greenery::tests::services::light
{

class LightTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        relay_mock = std::make_shared<open_greenery::mock::relay::StatefulRelayMock>();
        config_provider_mock =
                std::make_shared<open_greenery::mock::dataflow::light::IAsyncConfigProviderMock>();
        current_time_provider_mock = std::make_shared<open_greenery::mock::dataflow::time::CurrentTimeProviderMock>();
        manual_control_provider_mock =
            std::make_shared<open_greenery::mock::dataflow::light::IAsyncManualControlProviderMock>();
        mode_provider_mock =
                std::make_shared<open_greenery::mock::dataflow::light::IAsyncModeProviderMock>();
        status_receiver_mock = std::make_shared<open_greenery::mock::dataflow::light::IAsyncStatusReceiverMock>();

        EXPECT_CALL(*relay_mock, enabled())
                .Times(AnyNumber());
        EXPECT_CALL(*current_time_provider_mock, get())
                .Times(AnyNumber())
                .WillRepeatedly(ReturnPointee(&current_time));
        expectHandlersAssignment();
        controller = std::make_unique<ogl::LightController>(
                relay_mock,
                config_provider_mock,
                current_time_provider_mock,
                manual_control_provider_mock,
                mode_provider_mock,
                status_receiver_mock
        );
        controller_finish = controller->start();
    }

    void TearDown() override
    {
        if (!controller)
        {
            return;
        }

        controller->stop();
        if (controller_finish->valid())
        {
            controller_finish->wait();
        }
    }

    void waitForHandling(std::chrono::milliseconds duration = DEFAULT_HANDLING_TIME)
    {
        std::this_thread::sleep_for(duration);
    }

    void expectHandlersAssignment()
    {
        EXPECT_CALL(*manual_control_provider_mock, onUpdate(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&emulate_manual_control));
        EXPECT_CALL(*mode_provider_mock, onUpdate(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&emulate_mode_update));
        EXPECT_CALL(*config_provider_mock, onUpdate(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&emulate_config_update));
        EXPECT_CALL(*status_receiver_mock, onRequest(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&emulate_status_request));
    }

    // Mocks
    std::shared_ptr<open_greenery::mock::relay::StatefulRelayMock> relay_mock;
    std::shared_ptr<open_greenery::mock::dataflow::light::IAsyncConfigProviderMock> config_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::time::CurrentTimeProviderMock> current_time_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::light::IAsyncManualControlProviderMock> manual_control_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::light::IAsyncModeProviderMock> mode_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::light::IAsyncStatusReceiverMock> status_receiver_mock;

    // Assigned handlers bu controller
    open_greenery::dataflow::common::AsyncReceive<ogdfl::Control> emulate_manual_control;
    open_greenery::dataflow::common::AsyncReceive<ogdfl::Mode> emulate_mode_update;
    open_greenery::dataflow::common::AsyncReceive<ogdfl::LightConfigRecord> emulate_config_update;
    open_greenery::dataflow::common::AsyncProvide<bool> emulate_status_request;

    std::unique_ptr<ogl::LightController> controller;
    std::optional<open_greenery::tools::FinishFuture> controller_finish;
    QTime current_time;
    static constexpr std::chrono::milliseconds DEFAULT_HANDLING_TIME {150ms};
};

TEST_F(LightTest, DisabledByDefault)
{
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, ManualEnable)
{
    EXPECT_CALL(*relay_mock, enable())
        .Times(1);
    emulate_manual_control(ogdfl::Control::ENABLE);
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(LightTest, ManualDisable)
{
    EXPECT_CALL(*relay_mock, disable())
          .Times(1);
    emulate_manual_control(ogdfl::Control::DISABLE);
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, ManualToggle)
{
    EXPECT_CALL(*relay_mock, toggle())
          .Times(2);
    emulate_manual_control(ogdfl::Control::TOGGLE);
    EXPECT_TRUE(relay_mock->enabled());
    emulate_manual_control(ogdfl::Control::TOGGLE);
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, HandleManualInAuto)
{
    emulate_mode_update(ogdfl::Mode::AUTO);
    {
        InSequence s;

        // Enable
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        // Disable
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
        // Toggle
        EXPECT_CALL(*relay_mock, toggle())
                .Times(1);
    }

    emulate_manual_control(ogdfl::Control::ENABLE);
    emulate_manual_control(ogdfl::Control::DISABLE);
    emulate_manual_control(ogdfl::Control::TOGGLE);
}

TEST_F(LightTest, AutoEnable)
{
    {
        InSequence s;
        // Manual disable
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
        // Auto enable
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
    }
    emulate_mode_update(ogdfl::Mode::AUTO);
    // Disable manually
    emulate_manual_control(ogdfl::Control::DISABLE);
    EXPECT_FALSE(relay_mock->enabled());
    // Enable automatically
    ogdfl::LightConfigRecord cfg;
    cfg.day_start = QTime(0, 5); // 00:05
    cfg.day_end = QTime(23, 59); // 23:59
    emulate_config_update(cfg);

    // Enable automatically
    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled()); // still disabled

    current_time = QTime(0, 5); // day start
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(LightTest, AutoDisable)
{
    {
        InSequence s;
        // Manual enable
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        // Auto disable
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
    }

    emulate_mode_update(ogdfl::Mode::AUTO);
    // Enable manually
    emulate_manual_control(ogdfl::Control::ENABLE);
    EXPECT_TRUE(relay_mock->enabled());
    // Disable automatically
    ogdfl::LightConfigRecord cfg;
    cfg.day_start = QTime(23, 59); // 23:59
    cfg.day_end = QTime(0, 5); // 00:05
    emulate_config_update(cfg);


    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled()); // still enabled

    current_time = QTime(0, 5); // day end
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, DontHandleAutoInManual)
{
    EXPECT_CALL(*relay_mock, enable())
            .Times(0);
    EXPECT_CALL(*relay_mock, disable())
            .Times(0);
    EXPECT_CALL(*relay_mock, toggle())
            .Times(0);

    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};
    emulate_config_update(cfg); // 00:01, 00:02
    emulate_mode_update(ogdfl::Mode::MANUAL);

    current_time = QTime(0, 1);
    waitForHandling();
    current_time = QTime(0, 2);
    waitForHandling();
}

TEST_F(LightTest, ManualByDefault)
{
    // Same as previous test, but without explicit Manual mode set

    EXPECT_CALL(*relay_mock, enable())
            .Times(0);
    EXPECT_CALL(*relay_mock, disable())
            .Times(0);
    EXPECT_CALL(*relay_mock, toggle())
            .Times(0);

    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};
    emulate_config_update(cfg); // 00:01, 00:02

    current_time = QTime(0, 1);
    waitForHandling();
    current_time = QTime(0, 2);
    waitForHandling();
}

TEST_F(LightTest, ChangeConfig)
{
    {
        InSequence s;
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
    }

    ogdfl::LightConfigRecord first_cfg {QTime(0, 1), QTime(0, 2)}; // 00:01, 00:02
    ogdfl::LightConfigRecord second_cfg {QTime(20, 1), QTime(20, 2)}; // 20:01, 20:02

    emulate_mode_update(ogdfl::Mode::AUTO);

    // With first config
    emulate_config_update(first_cfg);
    current_time = QTime(0, 1);
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled());

    current_time = QTime(0, 2);
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled());

    // With second config
    emulate_config_update(second_cfg);
    current_time = QTime(20, 1);
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled());

    current_time = QTime(20, 2);
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, AutoControlDuplication)
{
    {
        InSequence s;
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
    }

    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};  // 00:01, 00:02
    emulate_config_update(cfg);
    emulate_mode_update(ogdfl::Mode::AUTO);

    current_time = QTime(0, 1); // day start
    waitForHandling(1000ms);
    EXPECT_TRUE(relay_mock->enabled());

    current_time = QTime(0, 2);  // day end
    waitForHandling(1000ms);
    EXPECT_FALSE(relay_mock->enabled());
}

}
