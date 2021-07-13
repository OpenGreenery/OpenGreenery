#ifndef I_CONFIG_PROVIDER_HPP
#define I_CONFIG_PROVIDER_HPP

#include <optional>
#include "ConfigRecord.hpp"

namespace open_greenery::dataflow::light
{

class IConfigProvider
{
public:
    virtual std::optional<LightConfigRecord> get() = 0;

    IConfigProvider() = default;
    virtual ~IConfigProvider() = default;
};

}

#endif //I_CONFIG_PROVIDER_HPP
