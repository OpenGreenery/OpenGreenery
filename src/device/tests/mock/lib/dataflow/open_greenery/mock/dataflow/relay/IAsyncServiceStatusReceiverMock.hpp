#pragma once

#include <open_greenery/dataflow/relay/Participants.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::relay
{

class IAsyncServiceStatusReceiverMock : public open_greenery::dataflow::relay::IAsyncServiceStatusReceiver
{
public:
    MOCK_METHOD(void,
                onRequest,
                (open_greenery::dataflow::common::AsyncProvide<open_greenery::dataflow::relay::ServiceStatus>),
                (override));
};

}
