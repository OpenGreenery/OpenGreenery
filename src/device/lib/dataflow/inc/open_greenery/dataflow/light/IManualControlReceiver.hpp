#ifndef I_CONTROL_RECEIVER_HPP
#define I_CONTROL_RECEIVER_HPP

#include "Control.hpp"

namespace open_greenery::dataflow::light
{

class IManualControlReceiver
{
public:
    virtual void set(Control control) = 0;

    IManualControlReceiver() = default;
    virtual ~IManualControlReceiver() = default;
};

}

#endif //I_CONTROL_RECEIVER_HPP
