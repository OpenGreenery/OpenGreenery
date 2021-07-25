#ifndef I_LIGHT_STATUS_PROVIDER_HPP
#define I_LIGHT_STATUS_PROVIDER_HPP

#include <optional>

namespace open_greenery::dataflow::light
{

class IStatusProvider
{
public:
    virtual std::optional<bool> get() = 0;

    IStatusProvider() = default;
    virtual ~IStatusProvider() = default;
};

}

#endif //I_LIGHT_STATUS_PROVIDER_HPP
