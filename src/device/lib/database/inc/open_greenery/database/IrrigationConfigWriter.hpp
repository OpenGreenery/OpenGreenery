#ifndef IRRIGATION_CONFIG_WRITER_HPP
#define IRRIGATION_CONFIG_WRITER_HPP

#include "DatabaseEntity.hpp"
#include <open_greenery/dataflow/irrigation/Participants.hpp>

namespace open_greenery::database
{

class IrrigationConfigWriter : public DatabaseEntity,
                               public open_greenery::dataflow::irrigation::IIrrigationDataReceiver
{
public:
    IrrigationConfigWriter(Table _table);

    void set(open_greenery::dataflow::irrigation::IrrigationConfigRecord _record) override;
};

}

#endif //IRRIGATION_CONFIG_WRITER_HPP
