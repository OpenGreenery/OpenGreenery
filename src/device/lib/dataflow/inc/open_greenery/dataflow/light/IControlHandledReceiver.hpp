#ifndef I_LIGHT_COMMAND_HANDLED_RECEIVER_HPP
#define I_LIGHT_COMMAND_HANDLED_RECEIVER_HPP

namespace open_greenery::dataflow::light
{

class IControlHandledReceiver
{
public:
    virtual void handled() = 0;

    IControlHandledReceiver() = default;
    virtual ~IControlHandledReceiver() = default;
};

}

#endif //I_LIGHT_COMMAND_HANDLED_RECEIVER_HPP
