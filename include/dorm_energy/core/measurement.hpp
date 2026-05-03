// include/dorm_energy/core/measurement.hpp
#pragma once

#include <chrono>
#include <vector>
#include <string>

namespace dorm_energy::core {

/**
 * @brief Измерение потребляемой мощности
 */
struct PowerMeasurement {
    std::chrono::system_clock::time_point timestamp;
    double      power_kw{0.0};  
    bool        is_anomaly{false}; 

    PowerMeasurement() = default;

    PowerMeasurement(std::chrono::system_clock::time_point ts, double power, bool anomaly = false)
        : timestamp{ts}, power_kw{power}, is_anomaly{anomaly} {}
};

/**
 * @brief Тип для хранения последовательности измерений мощности 
 */
using SimulationData = std::vector<PowerMeasurement>; // просто псевдоним для удобства использования

/**
 * @brief Преобразует PowerMeasurement в удобную строку для вывода
 * @return строка в формате "YYYY-MM-DD HH:MM | X.XX кВт | аномалия: ДА/нет"
 */
std::string to_string(const PowerMeasurement &m); // const PowerMeasurement& m - ссылка на обьект который нельязи изменять

} // namespace dorm_energy::core