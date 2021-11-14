#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include <open_greenery/relay/RelayController.hpp>
#include <open_greenery/dataflow/common/IAsyncReceiver.hpp>
#include <open_greenery/dataflow/common/IAsyncProvider.hpp>
#include <open_greenery/mock/dataflow/relay/IAsyncConfigProviderMock.hpp>
#include <open_greenery/mock/dataflow/relay/IAsyncManualControlProviderMock.hpp>
#include <open_greenery/mock/dataflow/relay/IAsyncModeProviderMock.hpp>
#include <open_greenery/mock/dataflow/relay/IAsyncRelayStatusRecevierMock.hpp>
#include <open_greenery/mock/dataflow/relay/IAsyncServiceStatusReceiverMock.hpp>
#include <open_greenery/mock/relay/StatefulRelayMock.hpp>
#include <open_greenery/mock/dataflow/time/CurrentTimeProviderMock.hpp>

namespace ogdfl = open_greenery::dataflow::relay;
namespace ogrc = open_greenery::relay;
using namespace std::chrono_literals;
using ::testing::_;
using ::testing::SaveArg;
using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::ReturnPointee;

namespace open_greenery::tests::services::relay
{

class RelayControllerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        relay_mock = std::make_shared<open_greenery::mock::relay::StatefulRelayMock>();
        config_provider_mock =
                std::make_shared<open_greenery::mock::dataflow::relay::IAsyncConfigProviderMock>();
        current_time_provider_mock = std::make_shared<open_greenery::mock::dataflow::time::CurrentTimeProviderMock>();
        manual_control_provider_mock =
                std::make_shared<open_greenery::mock::dataflow::relay::IAsyncManualControlProviderMock>();
        mode_provider_mock =
                std::make_shared<open_greenery::mock::dataflow::relay::IAsyncModeProviderMock>();
        relay_status_receiver_mock =
                std::make_shared<open_greenery::mock::dataflow::relay::IAsyncRelayStatusReceiverMock>();
        service_status_receiver_mock =
                std::make_shared<open_greenery::mock::dataflow::relay::IAsyncServiceStatusReceiverMock>();

        EXPECT_CALL(*relay_mock, enabled())
                .Times(AnyNumber());
        EXPECT_CALL(*current_time_provider_mock, get())
                .Times(AnyNumber())
                .WillRepeatedly(ReturnPointee(&current_time));
        expectHandlersAssignment();
        controller = std::make_unique<ogrc::RelayController>(
                relay_mock,
                config_provider_mock,
                current_time_provider_mock,
                manual_control_provider_mock,
                mode_provider_mock,
                relay_status_receiver_mock,
                service_status_receiver_mock
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
                .WillOnce(SaveArg<0>(&fnEmulateManualControl));
        EXPECT_CALL(*mode_provider_mock, onUpdate(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&fnEmulateModeUpdate));
        EXPECT_CALL(*config_provider_mock, onUpdate(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&fnEmulateConfigUpdate));
        EXPECT_CALL(*relay_status_receiver_mock, onRequest(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&fnEmulateRelayStatusRequest));
        EXPECT_CALL(*service_status_receiver_mock, onRequest(_))
                .Times(1)
                .WillOnce(SaveArg<0>(&fnEmulateServiceStatusRequest));
    }

    // Mocks
    std::shared_ptr<open_greenery::mock::relay::StatefulRelayMock> relay_mock;
    std::shared_ptr<open_greenery::mock::dataflow::relay::IAsyncConfigProviderMock> config_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::time::CurrentTimeProviderMock> current_time_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::relay::IAsyncManualControlProviderMock> manual_control_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::relay::IAsyncModeProviderMock> mode_provider_mock;
    std::shared_ptr<open_greenery::mock::dataflow::relay::IAsyncRelayStatusReceiverMock> relay_status_receiver_mock;
    std::shared_ptr<open_greenery::mock::dataflow::relay::IAsyncServiceStatusReceiverMock> service_status_receiver_mock;

    // Assigned handlers by controller
    open_greenery::dataflow::common::AsyncReceive<ogdfl::Control> fnEmulateManualControl;
    open_greenery::dataflow::common::AsyncReceive<ogdfl::Mode> fnEmulateModeUpdate;
    open_greenery::dataflow::common::AsyncReceive<ogdfl::Config> fnEmulateConfigUpdate;
    open_greenery::dataflow::common::AsyncProvide<bool> fnEmulateRelayStatusRequest;
    open_greenery::dataflow::common::AsyncProvide<ogdfl::ServiceStatus> fnEmulateServiceStatusRequest;

    std::unique_ptr<ogrc::RelayController> controller;
    std::optional<open_greenery::tools::FinishFuture> controller_finish;
    QTime current_time;
    static constexpr std::chrono::milliseconds DEFAULT_HANDLING_TIME{150ms};
};

TEST_F(RelayControllerTest, DisabledByDefault)
{
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, ManualEnable)
{
    EXPECT_CALL(*relay_mock, enable())
            .Times(1);
    fnEmulateManualControl(ogdfl::Control::ENABLE);
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, ManualDisable)
{
    EXPECT_CALL(*relay_mock, disable())
            .Times(1);
    fnEmulateManualControl(ogdfl::Control::DISABLE);
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, ManualToggle)
{
    EXPECT_CALL(*relay_mock, toggle())
            .Times(2);
    fnEmulateManualControl(ogdfl::Control::TOGGLE);
    EXPECT_TRUE(relay_mock->enabled());
    fnEmulateManualControl(ogdfl::Control::TOGGLE);
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, HandleManualInAuto)
{
    fnEmulateModeUpdate(ogdfl::Mode::AUTO);
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

    fnEmulateManualControl(ogdfl::Control::ENABLE);
    fnEmulateManualControl(ogdfl::Control::DISABLE);
    fnEmulateManualControl(ogdfl::Control::TOGGLE);
}

TEST_F(RelayControllerTest, AutoEnable)
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
    fnEmulateModeUpdate(ogdfl::Mode::AUTO);
    // Disable manually
    fnEmulateManualControl(ogdfl::Control::DISABLE);
    EXPECT_FALSE(relay_mock->enabled());
    // Enable automatically
    ogdfl::Config cfg;
    cfg.day_start = QTime(0, 5); // 00:05
    cfg.day_end = QTime(23, 59); // 23:59
    fnEmulateConfigUpdate(cfg);

    // Enable automatically
    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled()); // still disabled

    current_time = QTime(0, 5); // day start
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, AutoDisable)
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

    fnEmulateModeUpdate(ogdfl::Mode::AUTO);
    // Enable manually
    fnEmulateManualControl(ogdfl::Control::ENABLE);
    EXPECT_TRUE(relay_mock->enabled());
    // Disable automatically
    ogdfl::Config cfg;
    cfg.day_start = QTime(23, 59); // 23:59
    cfg.day_end = QTime(0, 5); // 00:05
    fnEmulateConfigUpdate(cfg);


    current_time = QTime(0, 4, 59, 999); // before 00:05
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled()); // still enabled

    current_time = QTime(0, 5); // day end
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, DontHandleAutoInManual)
{
    EXPECT_CALL(*relay_mock, enable())
            .Times(0);
    EXPECT_CALL(*relay_mock, disable())
            .Times(0);
    EXPECT_CALL(*relay_mock, toggle())
            .Times(0);

    ogdfl::Config cfg{QTime(0, 1), QTime(0, 2)};
    fnEmulateConfigUpdate(cfg); // 00:01, 00:02
    fnEmulateModeUpdate(ogdfl::Mode::MANUAL);

    current_time = QTime(0, 1);
    waitForHandling();
    current_time = QTime(0, 2);
    waitForHandling();
}

TEST_F(RelayControllerTest, ManualByDefault)
{
    // Same as previous test, but without explicit Manual mode set

    EXPECT_CALL(*relay_mock, enable())
            .Times(0);
    EXPECT_CALL(*relay_mock, disable())
            .Times(0);
    EXPECT_CALL(*relay_mock, toggle())
            .Times(0);

    ogdfl::Config cfg{QTime(0, 1), QTime(0, 2)};
    fnEmulateConfigUpdate(cfg); // 00:01, 00:02

    current_time = QTime(0, 1);
    waitForHandling();
    current_time = QTime(0, 2);
    waitForHandling();
}

TEST_F(RelayControllerTest, ChangeConfig)
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

    ogdfl::Config first_cfg{QTime(0, 1), QTime(0, 2)}; // 00:01, 00:02
    ogdfl::Config second_cfg{QTime(20, 1), QTime(20, 2)}; // 20:01, 20:02

    fnEmulateModeUpdate(ogdfl::Mode::AUTO);

    // With first config
    fnEmulateConfigUpdate(first_cfg);
    current_time = QTime(0, 1);
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled());

    current_time = QTime(0, 2);
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled());

    // With second config
    fnEmulateConfigUpdate(second_cfg);
    current_time = QTime(20, 1);
    waitForHandling();
    EXPECT_TRUE(relay_mock->enabled());

    current_time = QTime(20, 2);
    waitForHandling();
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, AutoControlDuplication)
{
    {
        InSequence s;
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
    }

    ogdfl::Config cfg{QTime(0, 1), QTime(0, 2)};  // 00:01, 00:02
    fnEmulateConfigUpdate(cfg);
    fnEmulateModeUpdate(ogdfl::Mode::AUTO);

    current_time = QTime(0, 1); // day start
    waitForHandling(1000ms);
    EXPECT_TRUE(relay_mock->enabled());

    current_time = QTime(0, 2);  // day end
    waitForHandling(1000ms);
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(RelayControllerTest, RelayStatusRequest)
{
    {
        InSequence s;
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
        EXPECT_CALL(*relay_mock, toggle())
                .Times(2);
    }

    for (const auto [command, expected_status] : std::array<std::tuple<ogdfl::Control, bool>, 4>{
            std::make_tuple(ogdfl::Control::ENABLE,  true),
            std::make_tuple(ogdfl::Control::DISABLE, false),
            std::make_tuple(ogdfl::Control::TOGGLE,  true),
            std::make_tuple(ogdfl::Control::TOGGLE,  false)
    })
    {
        // Emulate manual control
        fnEmulateManualControl(command);
        // Request relay status
        const bool actual_status{fnEmulateRelayStatusRequest()};
        EXPECT_EQ(expected_status, actual_status);
    }
}

TEST_F(RelayControllerTest, ServiceStatusRequest)
{
    {
        InSequence s;
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
        EXPECT_CALL(*relay_mock, toggle())
                .Times(2);
        EXPECT_CALL(*relay_mock, enable())
                .Times(1);
        EXPECT_CALL(*relay_mock, disable())
                .Times(1);
        EXPECT_CALL(*relay_mock, toggle())
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
            fnEmulateModeUpdate(expected_mode);
            fnEmulateManualControl(command);
            fnEmulateConfigUpdate(expected_config);

            const auto actual_status = fnEmulateServiceStatusRequest();
            EXPECT_EQ(expected_mode, actual_status.mode);
            EXPECT_EQ(expected_status, actual_status.relay_enabled);
            EXPECT_EQ(expected_config.day_start, actual_status.config.day_start);
            EXPECT_EQ(expected_config.day_end, actual_status.config.day_end);
        }
    }
}

}
