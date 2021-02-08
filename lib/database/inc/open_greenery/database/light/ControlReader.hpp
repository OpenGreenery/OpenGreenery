#ifndef OPEN_GREENERY_CONTROLREADER_HPP
#define OPEN_GREENERY_CONTROLREADER_HPP

#include "open_greenery/dataflow/light/IControlProvider.hpp"

namespace open_greenery::database::light
{

class ControlReader : public open_greenery::dataflow::light::IControlProvider
{
public:
    std::optional<open_greenery::dataflow::light::Control> get() override;
};

}

#endif //OPEN_GREENERY_CONTROLREADER_HPP
