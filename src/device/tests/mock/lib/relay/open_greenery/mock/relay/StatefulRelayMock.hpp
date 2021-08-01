#ifndef RELAY_MOCK_HPP
#define RELAY_MOCK_HPP

#include <open_greenery/relay/IRelay.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::relay
{

class StatefulRelayMock : public open_greenery::relay::IRelay
{
public:
    MOCK_METHOD(void, enable, (), (override));
    MOCK_METHOD(void, disable, (), (override));
    MOCK_METHOD(bool, enabled, (), (const, override));
    MOCK_METHOD(void, toggle, (), (override));

    StatefulRelayMock()
        :m_enabled(false)
    {
        ON_CALL(*this, enable()).WillByDefault(testing::Invoke([this]{m_enabled=true;}));
        ON_CALL(*this, disable()).WillByDefault(testing::Invoke([this]{m_enabled=false;}));
        ON_CALL(*this, toggle()).WillByDefault(testing::Invoke([this]{m_enabled=!m_enabled;}));
        ON_CALL(*this, enabled()).WillByDefault(testing::ReturnPointee(&m_enabled));
    }

private:
    bool m_enabled;
};

}

#endif //RELAY_MOCK_HPP
