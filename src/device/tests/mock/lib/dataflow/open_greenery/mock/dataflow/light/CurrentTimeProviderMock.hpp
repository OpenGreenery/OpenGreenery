#ifndef CURRENT_TIME_PROVIDER_MOCK_HPP
#define CURRENT_TIME_PROVIDER_MOCK_HPP

#include <open_greenery/dataflow/light/ICurrentTimeProvider.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::light
{

class CurrentTimeProviderMock : public open_greenery::dataflow::light::ICurrentTimeProvider
{
public:
    MOCK_METHOD(QTime, get, (), (override));
};

}

#endif //CURRENT_TIME_PROVIDER_MOCK_HPP
