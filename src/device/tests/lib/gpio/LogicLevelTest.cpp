#include <gtest/gtest.h>
#include <open_greenery/gpio/LogicLevel.hpp>

namespace open_greenery::tests::gpio
{

namespace ogio = open_greenery::gpio;

TEST(LogicLevelTest, LogicalNotOperator)
{
    EXPECT_EQ(!ogio::LogicLevel::LOW, ogio::LogicLevel::HIGH);
    EXPECT_EQ(!ogio::LogicLevel::HIGH, ogio::LogicLevel::LOW);
}

}
