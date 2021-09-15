#ifndef WET_STATE_HPP
#define WET_STATE_HPP

#include "State.hpp"
#include <open_greenery/dataflow/irrigation/Participants.hpp>

namespace open_greenery::irrigation::moisture
{

class WetState final : public State
{
public:
    WetState(const open_greenery::dataflow::irrigation::IrrigationConfigRecord & _config);

    StateName handleSoilMoisture(std::int16_t _moisture) override;
    StateName state() override;
};

}

#endif //WET_STATE_HPP
