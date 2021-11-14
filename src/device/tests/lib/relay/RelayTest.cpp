#include <gtest/gtest.h>
#include <open_greenery/mock/gpio/OutputPinMock.hpp>
#include <open_greenery/relay/Relay.hpp>

namespace ogio = open_greenery::gpio;
namespace ogr = open_greenery::relay;

namespace open_greenery::tests::relay
{

class RelayTest : public ::testing::TestWithParam<ogio::LogicLevel>
{
protected:
    explicit RelayTest()
        :ACTIVE_LEVEL(GetParam()){}

    void SetUp() override
    {
        m_output_pin_mock = std::make_shared<open_greenery::mock::gpio::OutputPinMock>();
        EXPECT_CALL(*m_output_pin_mock, write(!ACTIVE_LEVEL))
                .Times(1);
        m_relay = std::make_unique<ogr::Relay>(m_output_pin_mock, ACTIVE_LEVEL);
    }

    void TearDown() override
    {
        EXPECT_CALL(*m_output_pin_mock, write(!ACTIVE_LEVEL))
                .Times(1);
        m_relay.reset();
    }

    const ogio::LogicLevel ACTIVE_LEVEL;
    std::unique_ptr<ogr::Relay> m_relay;
    std::shared_ptr<open_greenery::mock::gpio::OutputPinMock> m_output_pin_mock;
};

TEST_P(RelayTest, DisabledByDefault)
{
    EXPECT_FALSE(m_relay->enabled());
}

TEST_P(RelayTest, Enable)
{
    EXPECT_CALL(*m_output_pin_mock, write(ACTIVE_LEVEL))
            .Times(1);
    m_relay->enable();
    EXPECT_TRUE(m_relay->enabled());
}

TEST_P(RelayTest, Disable)
{
    EXPECT_CALL(*m_output_pin_mock, write(!ACTIVE_LEVEL))
            .Times(1);
    m_relay->disable();
    EXPECT_FALSE(m_relay->enabled());
}

TEST_P(RelayTest, Toggle)
{
    {
        testing::InSequence s;
        EXPECT_CALL(*m_output_pin_mock, write(ACTIVE_LEVEL))
                .Times(1);
        EXPECT_CALL(*m_output_pin_mock, write(!ACTIVE_LEVEL))
                .Times(1);
    }

    const bool first{m_relay->enabled()};
    m_relay->toggle();
    const bool second{m_relay->enabled()};
    m_relay->toggle();
    const bool third{m_relay->enabled()};

    EXPECT_NE(first, second);
    EXPECT_NE(second, third);
    EXPECT_EQ(first, third);
}

INSTANTIATE_TEST_SUITE_P(
        ActiveLevels,
        RelayTest,
        testing::Values(ogio::LogicLevel::LOW, ogio::LogicLevel::HIGH),
        [](const testing::TestParamInfo<RelayTest::ParamType> & info)
        {
            return (info.param == ogio::LogicLevel::LOW) ? "LOW" : "HIGH";
        }
);

}
