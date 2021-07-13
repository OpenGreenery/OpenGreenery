#ifndef CURRENT_TIME_PROVIDER_HPP
#define CURRENT_TIME_PROVIDER_HPP

#include "open_greenery/dataflow/light/ICurrentTimeProvider.hpp"

namespace open_greenery::light
{

namespace ogl = open_greenery::dataflow::light;

class CurrentTimeProvider : public ogl::ICurrentTimeProvider
{
public:
    QTime get() override;
};

}

#endif //CURRENT_TIME_PROVIDER_HPP
