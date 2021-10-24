#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/adc/ADS1115Reader.hpp"

namespace open_greenery::adc
{

namespace ogdfi = open_greenery::dataflow::irrigation;

ADCFactory & ADCFactory::getInstance()
{
    static ADCFactory instance;
    return instance;
}

std::shared_ptr<ogdfi::ISensorReadProvider> ADCFactory::getReader(
        const ogdr::ADS1115::Address _adr,
        const ogdr::ADS1115::MUX _mux)
{
    if (m_ads1115.find(_adr) == m_ads1115.end())
    {
        m_ads1115[_adr] = ADS1115Storage();
    }
    ADS1115Storage & storage = m_ads1115[_adr];
    std::unique_ptr<ogdr::ADS1115> & adc = storage.adc;
    std::map<ogdr::ADS1115::MUX, std::shared_ptr<ogdfi::ISensorReadProvider>> & readers = storage.readers;

    if (!adc)
    {
        adc = std::make_unique<ogdr::ADS1115>(_adr);
    }

    if (readers.find(_mux) == readers.end())
    {
        readers[_mux] = std::make_shared<ADS1115Reader>(*adc, _mux);
    }

    return readers[_mux];
}

}
