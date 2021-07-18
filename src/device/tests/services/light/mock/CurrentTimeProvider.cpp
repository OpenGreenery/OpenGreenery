#include <QtWidgets/QTimeEdit>
#include "CurrentTimeProvider.hpp"

namespace open_greenery::tests::services::light::mock
{

CurrentTimeProvider::CurrentTimeProvider(QTime start_time)
    :m_current_time(start_time) {}

CurrentTimeProvider::CurrentTimeProvider()
    :CurrentTimeProvider(QTime(0, 0)) {}

QTime CurrentTimeProvider::get()
{
    return m_current_time;
}

void CurrentTimeProvider::set(QTime new_time)
{
    m_current_time = new_time;
}

}
