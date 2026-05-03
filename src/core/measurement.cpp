// src/core/measurement.cpp
#include "dorm_energy/core/measurement.hpp"
#include <format>

namespace dorm_energy::core {

std::string to_string(const PowerMeasurement& m)
{
    return std::format(" {:%Y-%m-%d %H:%M} | {:.2f} кВт | аномалия: {}",
                       m.timestamp,
                       m.power_kw,
                       m.is_anomaly ? "ДА" : "нет");
}

} // namespace dorm_energy::core