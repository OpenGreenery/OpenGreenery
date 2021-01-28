#include <gtest/gtest.h>
#include <chrono>
#include <memory>
#include <vector>
#include <open_greenery/irrigation/IrrigationController.hpp>
#include <open_greenery/dataflow/ISensorReadProvider.hpp>
#include <open_greenery/dataflow/IrrigationConfigRecord.hpp>
#include "open_greenery/pump/IPump.hpp"

class ConstantSensorReadProviderMock : public open_greenery::dataflow::ISensorReadProvider
{
public:
    ConstantSensorReadProviderMock(std::int16_t _soil_moisture)
        :m_soil_moisture(_soil_moisture)
    {}

    std::int16_t read() const override
    {
        return m_soil_moisture;
    }

private:
    const std::int16_t m_soil_moisture;
};

class PumpMock : public open_greenery::pump::IPump
{
public:
    using Milliliter = std::uint16_t;

    PumpMock(Milliliter _ml_per_min)
        :m_ml_per_min(_ml_per_min)
    {}

    void water(std::chrono::milliseconds _dur) override
    {
        const auto dur_sec = std::chrono::duration_cast<std::chrono::seconds>(_dur);
        m_activations.push_back(dur_sec.count() * m_ml_per_min);
    }

    void water(Milliliter _vol) override
    {
        m_activations.push_back(_vol);
    }

    const std::vector<Milliliter> & activations()
    {
        return m_activations;
    }

private:
    const Milliliter m_ml_per_min;

    std::vector<Milliliter> m_activations;
};

TEST_F(IrrigationTest, )
{

}
