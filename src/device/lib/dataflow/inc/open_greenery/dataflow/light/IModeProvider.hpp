#ifndef I_MODE_PROVIDER_HPP
#define I_MODE_PROVIDER_HPP

#include <optional>

namespace open_greenery::dataflow::light
{

enum class Mode: std::uint8_t {AUTO, MANUAL};

class IModeProvider
{
public:
    virtual std::optional<Mode> get() = 0;

    IModeProvider() = default;
    virtual ~IModeProvider() = default;
};

}

#endif //I_MODE_PROVIDER_HPP
