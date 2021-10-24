#ifndef ADC_FACTORY_HPP
#define ADC_FACTORY_HPP

#include <map>
#include <memory>
#include <vector>
#include <open_greenery/dataflow/irrigation/Participants.hpp>
#include <open_greenery/driver/ads1115/ADS1115.hpp>

namespace open_greenery
{
namespace adc
{


class ADCFactory
{
public:
    static ADCFactory & getInstance();

    ADCFactory(const ADCFactory &) = delete;
    ADCFactory & operator=(ADCFactory &) = delete;

    std::shared_ptr<open_greenery::dataflow::irrigation::ISensorReadProvider> getReader(
            const open_greenery::driver::ADS1115::Address _adr,
            const open_greenery::driver::ADS1115::MUX _mux);

private:
    ADCFactory() = default;

    struct ADS1115Storage
    {
        std::unique_ptr<open_greenery::driver::ADS1115> adc;
        std::map<
            open_greenery::driver::ADS1115::MUX,
            std::shared_ptr<open_greenery::dataflow::irrigation::ISensorReadProvider>> readers;
    };
    std::map<open_greenery::driver::ADS1115::Address, ADS1115Storage> m_ads1115;
};

}
}

#endif //ADC_FACTORY_HPP
