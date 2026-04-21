// src/core/measurement.cpp
#include "dorm_energy/core/measurement.hpp"
#include <format>
#include <ctime>

namespace dorm_energy::core {

std::string to_string(const PowerMeasurement& m)
{
    std::tm tm_buf; 
    
    if (localtime_s(&tm_buf, &m.timestamp) != 0) {
        return "Ошибка времени";
    }

    return std::format("{:04}-{:02}-{:02} {:02}:{:02} | {:.2f} кВт | аномалия: {}",
        tm_buf.tm_year + 1900,
        tm_buf.tm_mon + 1,
        tm_buf.tm_mday,
        tm_buf.tm_hour,
        tm_buf.tm_min,
        m.power_kw,
        m.is_anomaly ? "ДА" : "нет");
}

} // namespace dorm_energy::core