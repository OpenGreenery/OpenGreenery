#ifndef ADC_FACTORY_HPP
#define ADC_FACTORY_HPP

#include <map>
#include <memory>
#include <vector>
#include <open_greenery/dataflow/ISensorReadProvider.hpp>
#include <open_greenery/driver/ads1115/ADS1115.hpp>

namespace open_greenery
{
namespace adc
{

namespace ogdf = open_greenery::dataflow;
namespace ogdr = open_greenery::driver;

class ADCFactory
{
public:
    static ADCFactory & getInstance();

    ADCFactory(const ADCFactory &) = delete;
    ADCFactory & operator=(ADCFactory &) = delete;

    std::shared_ptr<ogdf::ISensorReadProvider> getReader(const ogdr::ADS1115::Address _adr,
                                          const ogdr::ADS1115::MUX _mux);

private:
    ADCFactory() = default;

    struct ADS1115Storage
    {
        std::unique_ptr<ogdr::ADS1115> adc;
        std::map<ogdr::ADS1115::MUX, std::shared_ptr<ogdf::ISensorReadProvider>> readers;
    };
    std::map<ogdr::ADS1115::Address, ADS1115Storage> m_ads1115;
};

}
}

#endif //ADC_FACTORY_HPP
