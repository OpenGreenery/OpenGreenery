#ifndef CURRENTTIMEPROVIDER_HPP
#define CURRENTTIMEPROVIDER_HPP

#include <open_greenery/dataflow/light/ICurrentTimeProvider.hpp>

namespace open_greenery::tests::services::light::mock
{

class CurrentTimeProvider : public open_greenery::dataflow::light::ICurrentTimeProvider
{
public:
    CurrentTimeProvider(); // 00:00
    explicit CurrentTimeProvider(QTime start_time);

    void set(QTime new_time);

    // ICurrentTimeProvider
    QTime get() override;

private:
    QTime m_current_time;
};

}

#endif //CURRENTTIMEPROVIDER_HPP
