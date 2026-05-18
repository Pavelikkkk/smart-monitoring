// include/dorm_energy/detection/anomaly_detector.hpp
#pragma once

#include <vector>
#include <chrono>

#include "../core/measurement.hpp"
#include "../interfaces.hpp"

namespace dorm_energy::detection
{
    struct AnomalyDetectorConfig 
    { // подумать над constexpr static для порогов
        constexpr static float high_power_threshold{40.0f};
        constexpr static float night_low_threshold{2.0f};

        constexpr static std::chrono::hh_mm_ss<std::chrono::minutes> night_start{std::chrono::hours{0}}; // сделаее красивее
        constexpr static std::chrono::hh_mm_ss<std::chrono::minutes> night_end{std::chrono::hours{6}};

        std::string timezone{"Europe/Moscow"}; // TODO: вынести в отдельный конфиг приложения позже

        AnomalyDetectorConfig() = default;
    };

    /**
     * @brief Детектор аномалий энергопотребления
     */
    class AnomalyDetector : public IAnomalyDetector
    {
    public:
        /**
         * @brief Конструктор с конфигурацией
         * @param config параметры порогов
         */
        explicit AnomalyDetector(AnomalyDetectorConfig config = {}); 

        /**
         * @brief Обнаруживает аномалии в переданных данных
         * @param data данные для анализа
         * @return количество найденных аномалий
         */
        int detect(core::SimulationData &data) const override;
        /**
         * @brief Проверяет, является ли конкретное измерение аномалией
         * @param m измерение для проверки
         * @return true, если это аномалия, иначе false
         */
        bool isAnomaly(const core::PowerMeasurement &m) const override;

        /**
         * @brief Возвращает список только аномальных измерений
         * @param data исходные данные
         * @return вектор аномалий
         */
        std::vector<core::PowerMeasurement> get_anomalies(const core::SimulationData &data) const override; 

    private:
        AnomalyDetectorConfig config_; 
    };

} // namespace dorm_energy::detection