#ifndef RELAY_HPP
#define RELAY_HPP

#include <open_greenery/relay/IRelay.hpp>

namespace open_greenery::tests::services::light::mock
{

class Relay : public open_greenery::relay::IRelay
{
public:
    void enable() override;
    void disable() override;
    void toggle() override;
    bool enabled() const override;

private:
    bool m_enabled;
};

}

#endif //RELAY_HPP
