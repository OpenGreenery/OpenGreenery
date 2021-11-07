#ifndef LIGHT_CONFIG_RECORD_HPP
#define LIGHT_CONFIG_RECORD_HPP

#include <QTime>

namespace open_greenery::dataflow::relay
{

struct Config
{
    QTime day_start;
    QTime day_end;
};

}

#endif //LIGHT_CONFIG_RECORD_HPP
