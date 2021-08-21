#ifndef I_PROVIDER_HPP
#define I_PROVIDER_HPP

namespace open_greenery::dataflow::common
{

template<class T>
class IProvider
{
public:
    virtual T get() = 0;

    IProvider() = default;
    virtual ~IProvider() = default;
};

}

#endif //I_PROVIDER_HPP
