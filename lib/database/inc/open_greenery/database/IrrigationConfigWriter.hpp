#ifndef IRRIGATION_CONFIG_WRITER_HPP
#define IRRIGATION_CONFIG_WRITER_HPP

#include "DatabaseEntity.hpp"
#include "IrrigationConfigRecord.hpp"

namespace open_greenery::database
{

class IrrigationConfigWriter : public DatabaseEntity
{
public:
    IrrigationConfigWriter(Table _table);

    void write(IrrigationConfigRecord _record);
};

}

#endif //IRRIGATION_CONFIG_WRITER_HPP
