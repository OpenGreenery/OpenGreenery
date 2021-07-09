#ifndef I_RELAY_HPP
#define I_RELAY_HPP

namespace open_greenery::relay
{

class IRelay
{
public:
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool enabled() const = 0;
    virtual void toggle() = 0;

    IRelay() = default;
    virtual ~IRelay() = default;
};

}

#endif //I_RELAY_HPP
