#ifndef I_MODE_PROVIDER_HPP
#define I_MODE_PROVIDER_HPP

#include <optional>
#include "Mode.hpp"

namespace open_greenery::dataflow::light
{

class IModeProvider
{
public:
    virtual std::optional<Mode> get() = 0;

    IModeProvider() = default;
    virtual ~IModeProvider() = default;
};

}

#endif //I_MODE_PROVIDER_HPP
