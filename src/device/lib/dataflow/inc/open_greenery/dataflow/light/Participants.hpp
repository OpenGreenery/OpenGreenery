#pragma once

#include "ConfigRecord.hpp"
#include "Control.hpp"
#include "Mode.hpp"
#include <open_greenery/dataflow/common/IAsyncProvider.hpp>
#include <open_greenery/dataflow/common/IAsyncReceiver.hpp>
#include <open_greenery/dataflow/common/IOptionalProvider.hpp>
#include <open_greenery/dataflow/common/IProvider.hpp>
#include <open_greenery/dataflow/common/IReceiver.hpp>

namespace open_greenery::dataflow::light
{

using IConfigProvider = open_greenery::dataflow::common::IProvider<LightConfigRecord>;
using IConfigOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<LightConfigRecord>;
using IConfigReceiver = open_greenery::dataflow::common::IReceiver<LightConfigRecord>;
using IAsyncConfigProvider = open_greenery::dataflow::common::IAsyncProvider<LightConfigRecord>;
using IAsyncConfigRecevier = open_greenery::dataflow::common::IAsyncReceiver<LightConfigRecord>;

using IManualControlProvider = open_greenery::dataflow::common::IProvider<Control>;
using IManualControlOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<Control>;
using IManualControlReceiver = open_greenery::dataflow::common::IReceiver<Control>;
using IAsyncManualControlProvider = open_greenery::dataflow::common::IAsyncProvider<Control>;
using IAsyncManualControlRecevier = open_greenery::dataflow::common::IAsyncReceiver<Control>;

using IModeProvider = open_greenery::dataflow::common::IProvider<Mode>;
using IModeProviderOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<Mode>;
using IModeReceiver = open_greenery::dataflow::common::IReceiver<Mode>;
using IAsyncModeProvider = open_greenery::dataflow::common::IAsyncProvider<Mode>;
using IAsyncModeRecevier = open_greenery::dataflow::common::IAsyncReceiver<Mode>;

using IStatusProvider = open_greenery::dataflow::common::IProvider<bool>;
using IStatusOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<bool>;
using IStatusReceiver = open_greenery::dataflow::common::IReceiver<bool>;
using IAsyncStatusProvider = open_greenery::dataflow::common::IAsyncProvider<bool>;
using IAsyncStatusRecevier = open_greenery::dataflow::common::IAsyncReceiver<bool>;

}
