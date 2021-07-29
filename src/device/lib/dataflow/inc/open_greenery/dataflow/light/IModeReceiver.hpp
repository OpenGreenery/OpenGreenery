#ifndef I_MODE_RECEIVER_HPP
#define I_MODE_RECEIVER_HPP

#include "Mode.hpp"

namespace open_greenery::dataflow::light
{

class IModeReceiver
{
public:
    virtual void set(Mode mode) = 0;

    IModeReceiver() = default;
    virtual ~IModeReceiver() = default;
};

}

#endif //I_MODE_RECEIVER_HPP
