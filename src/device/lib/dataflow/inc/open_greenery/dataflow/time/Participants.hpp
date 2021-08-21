#pragma once

#include <QTime>
#include <QDateTime>
#include <open_greenery/dataflow/common/IProvider.hpp>

namespace open_greenery::dataflow::time
{

using ICurrentTimeProvider = open_greenery::dataflow::common::IProvider<QTime>;
using ICurrentDateTimeProvider = open_greenery::dataflow::common::IProvider<QDateTime>;

}
