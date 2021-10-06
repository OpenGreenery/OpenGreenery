#ifndef CURRENT_TIME_PROVIDER_HPP
#define CURRENT_TIME_PROVIDER_HPP

#include "open_greenery/dataflow/time/Participants.hpp"

namespace open_greenery::light
{

class CurrentTimeProvider : public open_greenery::dataflow::time::ICurrentTimeProvider
{
public:
    QTime get() override;
};

}

#endif //CURRENT_TIME_PROVIDER_HPP
