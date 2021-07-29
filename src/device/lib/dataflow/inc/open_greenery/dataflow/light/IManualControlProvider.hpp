#ifndef I_CONTROL_PROVIDER_HPP
#define I_CONTROL_PROVIDER_HPP

#include <optional>
#include "Control.hpp"

namespace open_greenery::dataflow::light
{

class IManualControlProvider
{
public:
    virtual std::optional<Control> get() = 0;

    IManualControlProvider() = default;
    virtual ~IManualControlProvider() = default;
};

}

#endif //I_CONTROL_PROVIDER_HPP
