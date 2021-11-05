#pragma once

#include "IAsyncReceiver.hpp"

namespace open_greenery::dataflow::common
{

template <typename T>
class AsyncReceiver : public IAsyncReceiver<T>
{
public:
    void onRequest(AsyncProvide<T> provide) final
    {
        m_provide = provide;
    }
protected:
    AsyncProvide<T> m_provide {};
};

}