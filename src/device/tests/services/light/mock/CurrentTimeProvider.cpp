#include <QtWidgets/QTimeEdit>
#include "CurrentTimeProvider.hpp"

namespace open_greenery::tests::services::light::mock
{

QTime CurrentTimeProvider::get()
{
    constexpr uint8_t SECS_IN_MIN {60u};
    m_current_time = m_current_time.addSecs(int(SECS_IN_MIN*m_minutes_delta));
    return m_current_time;
}

CurrentTimeProvider::CurrentTimeProvider(QTime start_time, int minutes_delta)
    :m_current_time(start_time),
    m_minutes_delta(minutes_delta) {}


}
