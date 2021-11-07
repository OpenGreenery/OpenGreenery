#pragma once

#include "ConfigRecord.hpp"
#include "Control.hpp"
#include "Mode.hpp"
#include <open_greenery/dataflow/common/IAsyncProvider.hpp>
#include <open_greenery/dataflow/common/IAsyncReceiver.hpp>
#include <open_greenery/dataflow/common/IOptionalProvider.hpp>
#include <open_greenery/dataflow/common/IProvider.hpp>
#include <open_greenery/dataflow/common/IReceiver.hpp>

namespace open_greenery::dataflow::relay
{

using IConfigProvider = open_greenery::dataflow::common::IProvider<Config>;
using IConfigOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<Config>;
using IConfigReceiver = open_greenery::dataflow::common::IReceiver<Config>;
using IAsyncConfigProvider = open_greenery::dataflow::common::IAsyncProvider<Config>;
using IAsyncConfigReceiver = open_greenery::dataflow::common::IAsyncReceiver<Config>;

using IManualControlProvider = open_greenery::dataflow::common::IProvider<Control>;
using IManualControlOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<Control>;
using IManualControlReceiver = open_greenery::dataflow::common::IReceiver<Control>;
using IAsyncManualControlProvider = open_greenery::dataflow::common::IAsyncProvider<Control>;
using IAsyncManualControlReceiver = open_greenery::dataflow::common::IAsyncReceiver<Control>;

using IModeProvider = open_greenery::dataflow::common::IProvider<Mode>;
using IModeProviderOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<Mode>;
using IModeReceiver = open_greenery::dataflow::common::IReceiver<Mode>;
using IAsyncModeProvider = open_greenery::dataflow::common::IAsyncProvider<Mode>;
using IAsyncModeReceiver = open_greenery::dataflow::common::IAsyncReceiver<Mode>;

using IStatusProvider = open_greenery::dataflow::common::IProvider<bool>;
using IStatusOptionalProvider = open_greenery::dataflow::common::IOptionalProvider<bool>;
using IStatusReceiver = open_greenery::dataflow::common::IReceiver<bool>;
using IAsyncStatusProvider = open_greenery::dataflow::common::IAsyncProvider<bool>;
using IAsyncStatusReceiver = open_greenery::dataflow::common::IAsyncReceiver<bool>;

}
