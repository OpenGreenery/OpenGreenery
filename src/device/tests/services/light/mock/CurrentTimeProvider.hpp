#ifndef CURRENTTIMEPROVIDER_HPP
#define CURRENTTIMEPROVIDER_HPP

#include <open_greenery/dataflow/light/ICurrentTimeProvider.hpp>

namespace open_greenery::tests::services::light::mock
{

class CurrentTimeProvider : public open_greenery::dataflow::light::ICurrentTimeProvider
{
public:
    /// Configures mock to provide new time every get() call
    /// \param start_time Time from which mock stars
    /// \param minutes_delta Time delta in minutes which will be added to previous returned
    CurrentTimeProvider(QTime start_time, int minutes_delta);

    // ICurrentTimeProvider
    QTime get() override;

private:
    QTime m_current_time;
    unsigned int m_minutes_delta;
};

}

#endif //CURRENTTIMEPROVIDER_HPP
