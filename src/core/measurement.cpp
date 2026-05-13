// src/core/measurement.cpp
#include <format>

#include "dorm_energy/core/measurement.hpp"

namespace dorm_energy::core
{

    std::string to_string(const PowerMeasurement &m)
    {
        return std::format(" {:%Y-%m-%d %H:%M} (час {}) | {:.2f} кВт | аномалия: {}",
                           m.timestamp, m.hour_of_day, m.power_kw,
                           m.is_anomaly ? "ДА" : "нет");
    }

} // namespace dorm_energy::core