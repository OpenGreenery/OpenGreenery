#ifndef I_CONTROL_PROVIDER_HPP
#define I_CONTROL_PROVIDER_HPP

#include <optional>

namespace open_greenery::dataflow::light
{

enum class Control: std::uint8_t {ENABLE, DISABLE, TOGGLE};

class IControlProvider
{
public:
    virtual std::optional<Control> get() = 0;

    IControlProvider() = default;
    virtual ~IControlProvider() = default;
};

}

#endif //I_CONTROL_PROVIDER_HPP
