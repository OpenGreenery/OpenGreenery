#ifndef ADS1115_READER_HPP
#define ADS1115_READER_HPP

#include <open_greenery/dataflow/ISensorReadProvider.hpp>
#include <open_greenery/driver/ads1115/ADS1115.hpp>

namespace open_greenery::adc
{

namespace ogdr = open_greenery::driver;

class ADS1115Reader final : public open_greenery::dataflow::ISensorReadProvider
{
public:
    ADS1115Reader(ogdr::ADS1115 & _adc, ogdr::ADS1115::MUX _mux);
    ~ADS1115Reader() override = default;

    std::int16_t read() const override;

private:
    ogdr::ADS1115 & m_adc;
    ogdr::ADS1115::MUX m_mux;
};

}

#endif //ADS1115_READER_HPP
