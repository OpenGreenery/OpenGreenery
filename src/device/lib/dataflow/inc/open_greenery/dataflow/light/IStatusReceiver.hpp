#ifndef I_LIGHT_STATUS_RECEIVER_HPP
#define I_LIGHT_STATUS_RECEIVER_HPP

namespace open_greenery::dataflow::light
{

class IStatusReceiver
{
public:
    virtual void set(bool _is_enabled) = 0;

    IStatusReceiver() = default;
    virtual ~IStatusReceiver() = default;
};

}

#endif //I_LIGHT_STATUS_RECEIVER_HPP
