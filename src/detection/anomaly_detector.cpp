// src/detection/anomaly_detector.cpp
#include <format>
#include <iostream>

#include "dorm_energy/detection/anomaly_detector.hpp"

namespace dorm_energy::detection
{

    AnomalyDetector::AnomalyDetector(AnomalyDetectorConfig config)
        : config_{std::move(config)}
    {
    }

    int AnomalyDetector::detect(core::SimulationData &data) const
    {
        int anomaly_count{0};

        for (auto &m : data)
        {
            bool is_anomaly{false};

            is_anomaly = isAnomaly(m);

            if (is_anomaly)
            {
                m.is_anomaly = true;
                anomaly_count++;
            }
        }

        return anomaly_count;
    }

    std::vector<core::PowerMeasurement>
    AnomalyDetector::get_anomalies(const core::SimulationData &data) const
    {
        std::vector<core::PowerMeasurement> anomalies;
        uint8_t anomalies_percent{10}; // TODO: вынести в конфиг и оптимизировать под данные
        anomalies.reserve(data.size() / anomalies_percent);

        for (const auto &m : data)
        {
            if (m.is_anomaly)
            {
                anomalies.push_back(m);
            }
        }

        return anomalies;
    }

    bool AnomalyDetector::isAnomaly(const core::PowerMeasurement &m) const
    {
        // Правило 1: Слишком высокое потребление
        if (m.power_kw > config_.high_power_threshold)
            return true;

        // Правило 2: Слишком низкое потребление ночью
        if (m.power_kw < AnomalyDetectorConfig::night_low_threshold &&
            m.hour_of_day >= 0 && m.hour_of_day <= 6)
            return true;

        return false;
    }
} // namespace dorm_energy::detection