#ifndef IRRIGATION_CONFIG_READER_HPP
#define IRRIGATION_CONFIG_READER_HPP

#include "DatabaseEntity.hpp"
#include "IrrigationConfigRecord.hpp"

namespace open_greenery::database
{

class IrrigationConfigReader : public DatabaseEntity
{
public:
    IrrigationConfigReader(Table _table);

    std::vector<IrrigationConfigRecord> read();
    IrrigationConfigRecord read(open_greenery::gpio::PinId _pin);

private:
    std::vector<IrrigationConfigRecord> read(const std::string & _query);
};

}

#endif //IRRIGATION_CONFIG_READER_HPP
