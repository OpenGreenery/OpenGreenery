#pragma once

#include <open_greenery/dataflow/light/Participants.hpp>
#include <gmock/gmock.h>

namespace open_greenery::mock::dataflow::light
{

class IAsyncConfigProviderMock : public open_greenery::dataflow::light::IAsyncConfigProvider
{
public:
    MOCK_METHOD(void,
                onUpdate,
                (open_greenery::dataflow::common::AsyncReceive
                        <open_greenery::dataflow::light::LightConfigRecord>),
                (override));
};

}
