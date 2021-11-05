#pragma once

#include "IAsyncProvider.hpp"

namespace open_greenery::dataflow::common
{

template<typename T>
class AsyncProvider : public IAsyncProvider<T>
{
public:
    void onUpdate(AsyncReceive<T> receive) final
    {
        m_receive = receive;
    }

private:
    AsyncReceive<T> m_receive {};
};

}
