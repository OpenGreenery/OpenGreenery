#ifndef IRRIGATION_CONFIG_READER_HPP
#define IRRIGATION_CONFIG_READER_HPP

#include <vector>
#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/irrigation/Participants.hpp>

namespace open_greenery::database
{

class IrrigationConfigReader : public DatabaseEntity,
                               public open_greenery::dataflow::irrigation::IIrrigationConfigDataProvider
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
