#ifndef PUMP_HPP
#define PUMP_HPP

#include "IPump.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include "open_greenery/relay/IRelay.hpp"

namespace open_greenery::pump
{

class Pump : public IPump
{
public:
    explicit Pump(std::shared_ptr<open_greenery::relay::IRelay> _relay);
    
    void water(std::chrono::milliseconds _dur) override;

    void water(std::uint16_t _vol_ml) override;

private:
    std::shared_ptr<open_greenery::relay::IRelay> m_relay;
};

}

#endif //PUMP_HPP
