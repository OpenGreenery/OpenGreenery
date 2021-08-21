#pragma once

#include "ConfigRecord.hpp"
#include "Control.hpp"
#include "Mode.hpp"
#include <open_greenery/dataflow/common/IOptionalProvider.hpp>
#include <open_greenery/dataflow/common/IProvider.hpp>
#include <open_greenery/dataflow/common/IReceiver.hpp>

namespace open_greenery::dataflow::light
{

using IConfigProvider = open_greenery::dataflow::common::IOptionalProvider<LightConfigRecord>;
using IConfigReceiver = open_greenery::dataflow::common::IReceiver<LightConfigRecord>;

using IManualControlProvider = open_greenery::dataflow::common::IOptionalProvider<Control>;
using IManualControlReceiver = open_greenery::dataflow::common::IReceiver<Control>;

using IModeProvider = open_greenery::dataflow::common::IOptionalProvider<Mode>;
using IModeReceiver = open_greenery::dataflow::common::IReceiver<Mode>;

using IStatusProvider = open_greenery::dataflow::common::IOptionalProvider<bool>;
using IStatusReceiver = open_greenery::dataflow::common::IReceiver<bool>;

}
