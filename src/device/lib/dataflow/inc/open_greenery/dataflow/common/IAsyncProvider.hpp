#pragma once

#include <functional>

namespace open_greenery::dataflow::common
{

template<typename T>
using AsyncReceive = std::function<void(T)>;

template<typename T>
class IAsyncProvider
{
public:
    virtual ~IAsyncProvider() = default;

    virtual void onUpdate(AsyncReceive<T> receive) = 0;
};

}
