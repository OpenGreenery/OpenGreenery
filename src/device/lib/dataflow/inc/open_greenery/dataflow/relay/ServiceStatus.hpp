#pragma once

#include <optional>
#include "Mode.hpp"
#include "ConfigRecord.hpp"

namespace open_greenery::dataflow::relay
{

struct ServiceStatus
{
    Mode mode {};
    bool relay_enabled {};
    Config config;
};

}
