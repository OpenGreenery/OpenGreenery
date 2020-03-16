#include <iostream>
#include <chrono>
#include <thread>
#include "open_greenery/adc/ADCFactory.hpp"
#include "spdlog/spdlog.h"

namespace oga = open_greenery::adc;
namespace ogd = open_greenery::driver;
namespace chr = std::chrono;
using clk = chr::system_clock;

int main()
{
    using ADC_MUX = ogd::ADS1115::MUX;
    // ADC reading sample
    spdlog::set_pattern("%v");
    constexpr ADC_MUX adc_ports [4] = {ADC_MUX::SINGLE_0,
                                      ADC_MUX::SINGLE_1,
                                      ADC_MUX::SINGLE_2,
                                      ADC_MUX::SINGLE_3};

    oga::ADCFactory & adc_factory = oga::ADCFactory::getInstance();
    std::map<ADC_MUX, std::shared_ptr<oga::IADCReader>> adc_readers;
    for (const auto port : adc_ports)
    {
        adc_readers[port] = adc_factory.getReader(ogd::ADS1115::Address::GND, port);
    }

    clk::time_point end = clk::now() + chr::seconds(30);
    try
    {
        while (clk::now() < end)
        {
            spdlog::info("A0: {:5d} | A1: {:5d} | A2: {:5d} | A3: {:5d}",
                         adc_readers[ADC_MUX::SINGLE_0]->read(),
                         adc_readers[ADC_MUX::SINGLE_1]->read(),
                         adc_readers[ADC_MUX::SINGLE_2]->read(),
                         adc_readers[ADC_MUX::SINGLE_3]->read());

            std::this_thread::sleep_for(chr::milliseconds(500));
        }
    }
    catch(const std::exception & e)
    {
        spdlog::error(e.what());
    }

    return 0;
}
