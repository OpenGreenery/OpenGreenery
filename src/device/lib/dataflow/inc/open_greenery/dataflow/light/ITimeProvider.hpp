#ifndef I_TIME_PROVIDER_HPP
#define I_TIME_PROVIDER_HPP

#include <QTime>

namespace open_greenery::dataflow::light
{

class ITimeProvider
{
public:
    virtual QTime get() = 0;

    ITimeProvider() = default;
    virtual ~ITimeProvider() = default;
};

}

#endif //I_TIME_PROVIDER_HPP
