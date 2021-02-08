#include "open_greenery/light/CurrentTimeProvider.hpp"

namespace open_greenery::light
{

QTime open_greenery::light::CurrentTimeProvider::get()
{
    return QTime::currentTime();
}

}
