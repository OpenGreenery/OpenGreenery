#ifndef I_OPTIONAL_PROVIDER_HPP
#define I_OPTIONAL_PROVIDER_HPP

#include <optional>

namespace open_greenery::dataflow::common
{

template<class T>
class IOptionalProvider
{
public:
    virtual std::optional<T> get() = 0;

    IOptionalProvider() = default;
    virtual ~IOptionalProvider() = default;
};

}

#endif //I_OPTIONAL_PROVIDER_HPP
