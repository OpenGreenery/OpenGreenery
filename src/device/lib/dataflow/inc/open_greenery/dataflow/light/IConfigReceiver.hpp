#ifndef I_CONFIG_RECEIVER_HPP
#define I_CONFIG_RECEIVER_HPP

#include "ConfigRecord.hpp"

namespace open_greenery::dataflow::light
{

class IConfigReceiver
{
public:
    virtual void set(LightConfigRecord record) = 0;

    IConfigReceiver() = default;
    virtual ~IConfigReceiver() = default;
};

}

#endif //I_CONFIG_RECEIVER_HPP
