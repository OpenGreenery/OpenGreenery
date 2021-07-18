#include <gtest/gtest.h>
#include <memory>

#include <open_greenery/light/LightController.hpp>
#include "mock/CurrentTimeProvider.hpp"
#include "mock/OptionalProvider.hpp"
#include "mock/Relay.hpp"
#include "mock/StatusReceiver.hpp"

namespace ogdfl = open_greenery::dataflow::light;
namespace ogl = open_greenery::light;

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
        current_time_provider_mock = std::make_shared<mock::CurrentTimeProvider>(QTime(0, 0), 1);
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
        controller->start();
    }

    void waitForHandling()
    {
        std::this_thread::sleep_for(std::chrono::seconds(HANDLING_TIME_SEC));
    }

    // Mocks
    std::shared_ptr<mock::Relay> relay_mock;
    std::shared_ptr<mock::OptionalProvider<ogdfl::IConfigProvider, ogdfl::LightConfigRecord>> config_provider_mock;
    std::shared_ptr<mock::CurrentTimeProvider> current_time_provider_mock;
    std::shared_ptr<mock::OptionalProvider<ogdfl::IManualControlProvider, ogdfl::Control>> manual_control_provider_mock;
    std::shared_ptr<mock::OptionalProvider<ogdfl::IModeProvider, ogdfl::Mode>> mode_provider_mock;
    std::shared_ptr<mock::StatusReceiver> status_receiver_mock;

    std::unique_ptr<ogl::LightController> controller;
    static constexpr int HANDLING_TIME_SEC {1};
};

TEST_F(LightTest, ManualEnable)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::MANUAL);
    manual_control_provider_mock->set(ogdfl::Control::ENABLE);

    waitForHandling();

    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses[0]);
}

TEST_F(LightTest, ManualDisable)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::MANUAL);
    manual_control_provider_mock->set(ogdfl::Control::DISABLE);

    waitForHandling();

    ASSERT_EQ(statuses.size(), 1);
    EXPECT_FALSE(statuses[0]);
}

TEST_F(LightTest, ManualToggle)
{
    auto & statuses = status_receiver_mock->getReceivedStatuses();
    ASSERT_EQ(statuses.size(), 0);

    mode_provider_mock->set(ogdfl::Mode::MANUAL);

    manual_control_provider_mock->set(ogdfl::Control::ENABLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 1);
    EXPECT_TRUE(statuses[0]);

    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 2);
    EXPECT_FALSE(statuses[1]);

    manual_control_provider_mock->set(ogdfl::Control::TOGGLE);
    waitForHandling();
    ASSERT_EQ(statuses.size(), 3);
    EXPECT_TRUE(statuses[2]);
}

}
