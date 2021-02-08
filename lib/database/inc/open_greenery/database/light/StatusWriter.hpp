#ifndef OPEN_GREENERY_STATUSWRITER_HPP
#define OPEN_GREENERY_STATUSWRITER_HPP

#include <open_greenery/dataflow/light/IStatusReceiver.hpp>

namespace open_greenery::database::light
{

class StatusWriter : public open_greenery::dataflow::light::IStatusReceiver
{
public:
    void set(bool _is_enabled) override;
};

}

#endif //OPEN_GREENERY_STATUSWRITER_HPP
