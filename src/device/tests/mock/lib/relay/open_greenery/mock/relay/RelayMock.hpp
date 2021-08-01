#ifndef RELAY_MOCK_HPP
#define RELAY_MOCK_HPP

#include <open_greenery/relay/IRelay.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::relay
{

class RelayMock : public open_greenery::relay::IRelay
{
public:
    MOCK_METHOD(void, enable, (), (override));
    MOCK_METHOD(void, disable, (), (override));
    MOCK_METHOD(bool, enabled, (), (const, override));
    MOCK_METHOD(void, toggle, (), (override));
};

}

#endif //RELAY_MOCK_HPP
