// include/dorm_energy/core/measurement.hpp
#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace dorm_energy::core
{
    /**
     * @brief Универсальное показание с любого датчика
     *
     * Это основная единица данных всего проекта.
     */

    using TimePoint = std::chrono::system_clock::time_point;

    struct SensorReading
    {
        TimePoint timestamp;

        std::string deviceId;

        std::string sensorType;

        double value;

        std::optional<bool> boolValue;

        std::string unit;
    };

    /**
     * @brief Основной тип для работы с коллекциями показаний
     */
    using ReadingsBatch = std::vector<SensorReading>;

    /**
     * @brief Получить час дня (0-23) из timestamp
     */
    int getHourOfDay(const SensorReading &reading);

    /**
     * @brief Красивое строковое представление для логов
     */
    std::string toString(const SensorReading &reading);

    /**
     * @brief Преобразование в JSON (для MQTT)
     */
    std::string toJson(const SensorReading &reading); // возможно вынести

} // namespace dorm_energy::core
