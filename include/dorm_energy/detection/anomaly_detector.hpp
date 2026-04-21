// include/dorm_energy/detection/anomaly_detector.hpp
#pragma once

#include "../core/measurement.hpp"
#include <vector>

namespace dorm_energy::detection {

/**
 * @brief Конфигурация детектора аномалий
 */
struct AnomalyDetectorConfig {
    double high_power_threshold = 40.0;    
    double night_low_threshold  = 2.0;      
    int    night_start_hour     = 0;        
    int    night_end_hour       = 6;       

    AnomalyDetectorConfig() = default;
};

/**
 * @brief Детектор аномалий энергопотребления
 */
class AnomalyDetector {
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
    int detect(core::SimulationData& data) const;

    /**
     * @brief Возвращает список только аномальных измерений
     * @param data исходные данные
     * @return вектор аномалий
     */
    std::vector<core::PowerMeasurement> get_anomalies(const core::SimulationData& data) const;

private:
    AnomalyDetectorConfig config_;
};

} // namespace dorm_energy::detection