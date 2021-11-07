#pragma once

#include <open_greenery/dataflow/relay/Participants.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::relay
{

class IAsyncModeProviderMock : public open_greenery::dataflow::relay::IAsyncModeProvider
{
public:
    MOCK_METHOD(void,
                onUpdate,
                (open_greenery::dataflow::common::AsyncReceive
                        <open_greenery::dataflow::relay::Mode>),
                (override));
};

}
