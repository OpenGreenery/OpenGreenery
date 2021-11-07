#pragma once

#include <open_greenery/dataflow/relay/Participants.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::relay
{

class IAsyncStatusReceiverMock : public open_greenery::dataflow::relay::IAsyncStatusReceiver
{
public:
    MOCK_METHOD(void,
                onRequest,
                (open_greenery::dataflow::common::AsyncProvide<bool>),
                (override));
};

}
