#ifndef IRRIGATION_CONFIG_WRITER_HPP
#define IRRIGATION_CONFIG_WRITER_HPP

#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/IrrigationConfigRecord.hpp>
#include <open_greenery/dataflow/IIrrigationConfigDataReceiver.hpp>

namespace open_greenery::database
{

class IrrigationConfigWriter : public DatabaseEntity,
                               public open_greenery::dataflow::IIrrigationDataReceiver
{
public:
    IrrigationConfigWriter(Table _table);

    void write(open_greenery::dataflow::IrrigationConfigRecord _record) override;
};

}

#endif //IRRIGATION_CONFIG_WRITER_HPP
