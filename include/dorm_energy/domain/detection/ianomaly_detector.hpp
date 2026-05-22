// include/dorm_energy/domain/detection/ianomaly_detector.hpp
#pragma once

#include "dorm_energy/core/measurement.hpp"
#include <vector>

namespace dorm_energy::detection
{

    /**
     * @brief Интерфейс для обнаружения аномалий энергопотребления
     * 
     * Реализации могут быть:
     * - Пороговые правила
     * - Статистические методы
     * - ML-модели
     */
    class IAnomalyDetector
    {
    public:
        virtual ~IAnomalyDetector() = default;

        /**
         * @brief Проверяет, является ли одно показание аномалией
         * @param reading показание датчика
         * @return true, если это аномалия
         */
        virtual bool isAnomaly(const core::SensorReading &reading) const = 0;

        /**
         * @brief Возвращает все аномальные показания из батча
         * @param data набор показаний
         * @return вектор только аномальных показаний (копии)
         */
        virtual std::vector<core::SensorReading> getAnomalies(const core::ReadingsBatch &data) const = 0;

        /**
         * @brief Подсчитывает количество аномалий в батче
         * @return количество найденных аномалий
         */
        virtual int countAnomalies(const core::ReadingsBatch &data) const = 0;
    };

} // namespace dorm_energy::domain::detection