#include <gtest/gtest.h>
#include <chrono>
#include <open_greenery/mock/relay/StatefulRelayMock.hpp>
#include <open_greenery/pump/Pump.hpp>

using namespace std::chrono_literals;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::InSequence;

namespace open_greenery::tests::lib::relay
{

class PumpTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        relay = std::make_shared<open_greenery::mock::relay::StatefulRelayMock>();
        EXPECT_CALL(*relay, enabled()).Times(AnyNumber());  // Uninteresting method
        pump.emplace(relay);
    }

    std::chrono::high_resolution_clock::time_point now()
    {
        return std::chrono::high_resolution_clock::now();
    }

    std::shared_ptr<open_greenery::mock::relay::StatefulRelayMock> relay;
    std::optional<open_greenery::pump::Pump> pump;
};

TEST_F(PumpTest, TimedWatering)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
    }

    const std::chrono::milliseconds EXPECTED_DURATION {10ms};

    const auto start_time = now();
    pump->water(EXPECTED_DURATION);
    const auto actual_duration = now() - start_time;

    EXPECT_EQ(
        std::chrono::duration_cast<std::chrono::milliseconds>(actual_duration).count(),
        EXPECTED_DURATION.count()
    );
}

TEST_F(PumpTest, VolumeWatering)
{
    {
        InSequence s;
        EXPECT_CALL(*relay, enable())
                .Times(1);
        EXPECT_CALL(*relay, disable())
                .Times(1);
    }

    constexpr unsigned int ML_MIN {100u};  // Pump performance
    const std::chrono::milliseconds MINUTE {1min};
    constexpr unsigned int DIVIDER (10u);

    const auto volume_ml = ML_MIN/DIVIDER;
    const auto expected_duration = MINUTE/DIVIDER;

    const auto start_time = now();
    pump->water(volume_ml);
    const auto actual_duration = now() - start_time;

    EXPECT_EQ(
    std::chrono::duration_cast<std::chrono::milliseconds>(actual_duration).count(),
    expected_duration.count()
    );
}

}
