#pragma once

#include "ConfigRecord.hpp"
#include "Control.hpp"
#include "Mode.hpp"
#include <open_greenery/dataflow/common/AsyncProvider.hpp>
#include <open_greenery/dataflow/common/AsyncReceiver.hpp>
#include <open_greenery/dataflow/common/IAsyncProvider.hpp>
#include <open_greenery/dataflow/common/IAsyncReceiver.hpp>
#include <open_greenery/dataflow/common/IOptionalProvider.hpp>
#include <open_greenery/dataflow/common/IProvider.hpp>
#include <open_greenery/dataflow/common/IReceiver.hpp>

namespace open_greenery::dataflow::light
{

using IConfigProvider = open_greenery::dataflow::common::IOptionalProvider<LightConfigRecord>;
using IConfigReceiver = open_greenery::dataflow::common::IReceiver<LightConfigRecord>;
using IAsyncConfigProvider = open_greenery::dataflow::common::IAsyncProvider<LightConfigRecord>;
using AsyncConfigProvider = open_greenery::dataflow::common::AsyncProvider<LightConfigRecord>;
using IAsyncConfigRecevier = open_greenery::dataflow::common::IAsyncReceiver<LightConfigRecord>;
using AsyncConfigRecevier = open_greenery::dataflow::common::AsyncReceiver<LightConfigRecord>;

using IManualControlProvider = open_greenery::dataflow::common::IOptionalProvider<Control>;
using IManualControlReceiver = open_greenery::dataflow::common::IReceiver<Control>;
using IAsyncManualControlProvider = open_greenery::dataflow::common::IAsyncProvider<Control>;
using AsyncManualControlProvider = open_greenery::dataflow::common::AsyncProvider<Control>;
using IAsyncManualControlRecevier = open_greenery::dataflow::common::IAsyncReceiver<Control>;
using AsyncManualControlRecevier = open_greenery::dataflow::common::AsyncReceiver<Control>;

using IModeProvider = open_greenery::dataflow::common::IOptionalProvider<Mode>;
using IModeReceiver = open_greenery::dataflow::common::IReceiver<Mode>;
using IAsyncModeProvider = open_greenery::dataflow::common::IAsyncProvider<Mode>;
using AsyncModeProvider = open_greenery::dataflow::common::AsyncProvider<Mode>;
using IAsyncModeRecevier = open_greenery::dataflow::common::IAsyncReceiver<Mode>;
using AsyncModeRecevier = open_greenery::dataflow::common::AsyncReceiver<Mode>;

using IStatusProvider = open_greenery::dataflow::common::IOptionalProvider<bool>;
using IStatusReceiver = open_greenery::dataflow::common::IReceiver<bool>;
using IAsyncStatusProvider = open_greenery::dataflow::common::IAsyncProvider<bool>;
using AsyncStatusProvider = open_greenery::dataflow::common::AsyncProvider<bool>;
using IAsyncStatusRecevier = open_greenery::dataflow::common::IAsyncReceiver<bool>;
using AsyncStatusRecevier = open_greenery::dataflow::common::AsyncReceiver<bool>;

}
