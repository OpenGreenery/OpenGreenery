#ifndef CURRENT_TIME_PROVIDER_MOCK_HPP
#define CURRENT_TIME_PROVIDER_MOCK_HPP

#include <open_greenery/dataflow/time/Participants.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::time
{

class CurrentTimeProviderMock : public open_greenery::dataflow::time::ICurrentTimeProvider
{
public:
    MOCK_METHOD(QTime, get, (), (override));
};

}

#endif //CURRENT_TIME_PROVIDER_MOCK_HPP
