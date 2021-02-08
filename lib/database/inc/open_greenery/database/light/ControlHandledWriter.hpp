#ifndef OPEN_GREENERY_CONTROLHANDLEDWRITER_HPP
#define OPEN_GREENERY_CONTROLHANDLEDWRITER_HPP

#include <open_greenery/dataflow/light/IControlHandledReceiver.hpp>

namespace open_greenery::database::light
{

class ControlHandledWriter : public open_greenery::dataflow::light::IControlHandledReceiver
{
public:
    void handled() override;
};

}

#endif //OPEN_GREENERY_CONTROLHANDLEDWRITER_HPP
