#ifndef WET_STATE_HPP
#define WET_STATE_HPP

#include "State.hpp"

namespace open_greenery::irrigation
{

class WetState final : public State
{
public:
    WetState(open_greenery::database::IrrigationConfigRecord _config);

    SystemState handleSoilMoisture(std::int16_t _moisture) override;
    SystemState state() override;
};

}

#endif //WET_STATE_HPP
