#pragma once

#include <functional>

namespace open_greenery::dataflow::common
{

template <typename T>
using AsyncProvide = std::function<T()>;

template <typename T>
class IAsyncReceiver
{
public:
    virtual ~IAsyncReceiver() = default;

    virtual void onRequest(AsyncProvide<T> provide) = 0;
};

}