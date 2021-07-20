#include <gtest/gtest.h>
#include <memory>

#include <open_greenery/light/LightController.hpp>
#include "mock/CurrentTimeProvider.hpp"
#include "mock/OptionalProvider.hpp"
#include "mock/Relay.hpp"
#include "mock/StatusReceiver.hpp"

namespace ogdfl = open_greenery::dataflow::light;
namespace ogl = open_greenery::light;
using namespace std::chrono_literals;

namespace open_greenery::tests::services::light
{

class LightTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        relay_mock = std::make_shared<mock::Relay>();
        config_provider_mock =
                std::make_shared<mock::OptionalProvider<ogdfl::IConfigProvider, ogdfl::LightConfigRecord>>();
        current_time_provider_mock = std::make_shared<mock::CurrentTimeProvider>();
        manual_control_provider_mock =
            std::make_shared<mock::OptionalProvider<ogdfl::IManualControlProvider, ogdfl::Control>>();
        mode_provider_mock =
                std::make_shared<mock::OptionalProvider<ogdfl::IModeProvider, ogdfl::Mode>>();
        status_receiver_mock = std::make_shared<mock::StatusReceiver>();

        controller = std::make_unique<ogl::LightController>(relay_mock,
                                                              config_provider_mock,
                                                              current_time_provider_mock,
                                                              manual_control_provider_mock,
                                                              mode_provider_mock,
                                                              status_receiver_mock);
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

    // Mocks
    std::shared_ptr<mock::Relay> relay_mock;
    std::shared_ptr<mock::OptionalProvider<ogdfl::IConfigProvider, ogdfl::LightConfigRecord>> config_provider_mock;
    std::shared_ptr<mock::CurrentTimeProvider> current_time_provider_mock;
    std::shared_ptr<mock::OptionalProvider<ogdfl::IManualControlProvider, ogdfl::Control>> manual_control_provider_mock;
    std::shared_ptr<mock::OptionalProvider<ogdfl::IModeProvider, ogdfl::Mode>> mode_provider_mock;
    std::shared_ptr<mock::StatusReceiver> status_receiver_mock;

    std::unique_ptr<ogl::LightController> controller;
    std::optional<open_greenery::tools::FinishFuture> controller_finish;
    static constexpr std::chrono::milliseconds DEFAULT_HANDLING_TIME {150ms};
};


TEST_F(LightTest, ManualEnable)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::MANUAL);
    manual_control_provider_mock->set(ogdfl::Control::ENABLE);

    waitForHandling();

    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(LightTest, ManualDisable)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::MANUAL);
    manual_control_provider_mock->set(ogdfl::Control::DISABLE);

    waitForHandling();

    ASSERT_EQ(statuses.size(), 1);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, ManualToggle)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::MANUAL);

    manual_control_provider_mock->set(ogdfl::Control::ENABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());

    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 3);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(LightTest, ManualControlDuplication)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    // Enable two times
    manual_control_provider_mock->set(ogdfl::Control::ENABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());
    manual_control_provider_mock->set(ogdfl::Control::ENABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    // Disable two times
    manual_control_provider_mock->set(ogdfl::Control::DISABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 3);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());
    manual_control_provider_mock->set(ogdfl::Control::DISABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 4);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());

    // Toggle two times
    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 5);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());
    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 6);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, HandleManualInAuto)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::AUTO);

    manual_control_provider_mock->set(ogdfl::Control::ENABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    manual_control_provider_mock->set(ogdfl::Control::DISABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());

    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 3);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(LightTest, AutoEnable)
{
    // Disable manually
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);
    manual_control_provider_mock->set(ogdfl::Control::DISABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());

    // Enable automatically
    ogdfl::LightConfigRecord cfg;
    cfg.day_start = QTime(0, 5); // 00:05
    cfg.day_end = QTime(23, 59); // 23:59
    config_provider_mock->set(cfg);
    mode_provider_mock->set(ogdfl::Mode::AUTO);

    current_time_provider_mock->set(QTime(0, 4, 59, 999)); // before 00:05
    waitForHandling();
    EXPECT_EQ(statuses.size(), 1); // nothing changed

    current_time_provider_mock->set(QTime(0, 5)); // day start
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());
}

TEST_F(LightTest, AutoDisable)
{
    // Enable manually
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);
    manual_control_provider_mock->set(ogdfl::Control::ENABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    // Disable automatically
    ogdfl::LightConfigRecord cfg;
    cfg.day_start = QTime(23, 59); // 23:59
    cfg.day_end = QTime(0, 5); // 00:05
    config_provider_mock->set(cfg);
    mode_provider_mock->set(ogdfl::Mode::AUTO);

    current_time_provider_mock->set(QTime(0, 4, 59, 999)); // before 00:05
    waitForHandling();
    EXPECT_EQ(statuses.size(), 1); // nothing changed

    current_time_provider_mock->set(QTime(0, 5)); // day end
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, DontHandleAutoInManual)
{
    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};
    config_provider_mock->set(cfg); // 00:01, 00:02
    mode_provider_mock->set(ogdfl::Mode::MANUAL);

    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0); // no any status change reports
    current_time_provider_mock->set(QTime(0, 1));
    waitForHandling();
    current_time_provider_mock->set(QTime(0, 2));
    waitForHandling();

    EXPECT_EQ(statuses.size(), 0); // nothing changed
}

TEST_F(LightTest, ManualByDefault)
{
    // Same as previous test, but without explicit Manual mode set

    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};
    config_provider_mock->set(cfg); // 00:01, 00:02

    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0); // no any status change reports
    current_time_provider_mock->set(QTime(0, 1));
    waitForHandling();
    current_time_provider_mock->set(QTime(0, 2));
    waitForHandling();

    EXPECT_EQ(statuses.size(), 0); // nothing changed
}

TEST_F(LightTest, ChangeConfig)
{
    ogdfl::LightConfigRecord first_cfg {QTime(0, 1), QTime(0, 2)}; // 00:01, 00:02
    ogdfl::LightConfigRecord second_cfg {QTime(20, 1), QTime(20, 2)}; // 20:01, 20:02

    mode_provider_mock->set(ogdfl::Mode::AUTO);

    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    // With first config
    config_provider_mock->set(first_cfg);
    current_time_provider_mock->set(QTime(0, 1));
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    current_time_provider_mock->set(QTime(0, 2));
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());

    // With second config
    config_provider_mock->set(second_cfg);
    current_time_provider_mock->set(QTime(20, 1));
    waitForHandling();
    ASSERT_EQ(statuses.size(), 3);
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    current_time_provider_mock->set(QTime(20, 2));
    waitForHandling();
    ASSERT_EQ(statuses.size(), 4);
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());
}

TEST_F(LightTest, AutoControlDuplication)
{
    ogdfl::LightConfigRecord cfg {QTime(0, 1), QTime(0, 2)};  // 00:01, 00:02
    config_provider_mock->set(cfg);
    mode_provider_mock->set(ogdfl::Mode::AUTO);

    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    current_time_provider_mock->set(QTime(0, 1)); // day start
    waitForHandling(1000ms);
    ASSERT_EQ(statuses.size(), 1); // only one enable event
    EXPECT_TRUE(statuses.back());
    EXPECT_TRUE(relay_mock->enabled());

    current_time_provider_mock->set(QTime(0, 2));  // day end
    waitForHandling(1000ms);
    ASSERT_EQ(statuses.size(), 2); // only one disable event
    EXPECT_FALSE(statuses.back());
    EXPECT_FALSE(relay_mock->enabled());
}

}
