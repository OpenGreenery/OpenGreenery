#ifndef IRRIGATION_CONFIG_READER_HPP
#define IRRIGATION_CONFIG_READER_HPP

#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/IrrigationConfigRecord.hpp>
#include <open_greenery/dataflow/IIrrigationConfigDataProvider.hpp>

namespace open_greenery::database
{

class IrrigationConfigReader : public DatabaseEntity,
                               public open_greenery::dataflow::IIrrigationConfigDataProvider
{
public:
    IrrigationConfigReader(Table _table);

    std::vector<open_greenery::dataflow::IrrigationConfigRecord> read() override;
    open_greenery::dataflow::IrrigationConfigRecord read(open_greenery::gpio::PinId _pin) override;

private:
    std::vector<open_greenery::dataflow::IrrigationConfigRecord> read(const std::string & _query);
};

}

#endif //IRRIGATION_CONFIG_READER_HPP
