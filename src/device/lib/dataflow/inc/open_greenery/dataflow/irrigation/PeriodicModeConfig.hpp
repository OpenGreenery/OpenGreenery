#pragma once

#include <chrono>

namespace open_greenery::dataflow::irrigation
{

using Millilitres = std::size_t;

struct PeriodicModeConfig
{
    std::chrono::milliseconds duration;
    Millilitres volume;
};

}