#include "open_greenery/sensor/AnalogSensorPublisher.hpp"

namespace open_greenery
{
namespace sensor
{

AnalogSensorPublisher::AnalogSensorPublisher(std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _provider)
    : m_sensor_provider(_provider),
      m_thread_continue(false)
{}

AnalogSensorPublisher::AnalogSensorPublisher(std::shared_ptr<open_greenery::dataflow::ISensorReadProvider> _provider,
                                             const std::chrono::milliseconds period)
    : AnalogSensorPublisher(_provider)
{
    enable(period);
}

AnalogSensorPublisher::~AnalogSensorPublisher()
{
    m_thread_continue = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void AnalogSensorPublisher::subscribe(Notificator _notificator)
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    m_notificators.push_back(_notificator);
}

void AnalogSensorPublisher::unsubscribe(Notificator _notificator)
{
    auto comp_lambda = [](Notificator _n1, Notificator _n2)
        {
            return _n1.target<NotificatorFuncPtr>() == _n2.target<NotificatorFuncPtr>();
        };
    auto predicate = [_notificator, comp_lambda](const Notificator & _n){return comp_lambda(_notificator, _n);};

    std::lock_guard<std::mutex> l(m_notificators_mutex);
    m_notificators.remove_if(predicate);
}

void AnalogSensorPublisher::enable(const std::chrono::milliseconds period)
{
    m_thread_continue = true;
    m_thread = std::thread(&AnalogSensorPublisher::update, this, period);
}

void AnalogSensorPublisher::disable()
{
    m_thread_continue = false;
}

void AnalogSensorPublisher::notify(const std::int16_t _val) const
{
    std::lock_guard<std::mutex> l(m_notificators_mutex);
    for (Notificator ntf : m_notificators)
    {
        ntf(_val);
    }
}

void AnalogSensorPublisher::update(const std::chrono::milliseconds period) const
{
    auto iteration_start = std::chrono::system_clock::now();
    while (m_thread_continue)
    {
        notify(m_sensor_provider->read());

        iteration_start += period;
        std::this_thread::sleep_until(iteration_start);
    }
}

}
}
