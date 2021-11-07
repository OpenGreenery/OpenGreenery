#include "open_greenery/relay/CurrentTimeProvider.hpp"

namespace open_greenery::relay
{

QTime open_greenery::relay::CurrentTimeProvider::get()
{
    return QTime::currentTime();
}

}
