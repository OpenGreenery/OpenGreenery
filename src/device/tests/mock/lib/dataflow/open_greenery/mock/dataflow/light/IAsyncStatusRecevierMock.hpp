#pragma once

#include <open_greenery/dataflow/light/Participants.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::light
{

class IAsyncStatusReceiverMock : public open_greenery::dataflow::light::IAsyncStatusReceiver
{
public:
    MOCK_METHOD(void,
                onRequest,
                (open_greenery::dataflow::common::AsyncProvide<bool>),
                (override));
};

}
