#ifndef EMPTYCURRENTTIMEPROVIDER_HPP
#define EMPTYCURRENTTIMEPROVIDER_HPP

#include <open_greenery/dataflow/light/ICurrentTimeProvider.hpp>

namespace open_greenery::tests::services::light::mock
{

class EmptyCurrentTimeProvider : public open_greenery::dataflow::light::ICurrentTimeProvider
{
    QTime get() override;
};

}

#endif //EMPTYCURRENTTIMEPROVIDER_HPP
