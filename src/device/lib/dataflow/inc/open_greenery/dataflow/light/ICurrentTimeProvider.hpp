#ifndef I_TIME_PROVIDER_HPP
#define I_TIME_PROVIDER_HPP

#include <QTime>

namespace open_greenery::dataflow::light
{

class ICurrentTimeProvider
{
public:
    virtual QTime get() = 0;

    ICurrentTimeProvider() = default;
    virtual ~ICurrentTimeProvider() = default;
};

}

#endif //I_TIME_PROVIDER_HPP
