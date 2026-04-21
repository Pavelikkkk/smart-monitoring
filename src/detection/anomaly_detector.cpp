// src/detection/anomaly_detector.cpp
#include "dorm_energy/detection/anomaly_detector.hpp"
#include <format>
#include <iostream>
#include <ctime>

namespace dorm_energy::detection {

AnomalyDetector::AnomalyDetector(AnomalyDetectorConfig config)
    : config_{std::move(config)}
{
}

int AnomalyDetector::detect(core::SimulationData& data) const
{
    int anomaly_count = 0;
    std::tm tm_buf; 

    for (auto& m : data) {
        if (localtime_s(&tm_buf, &m.timestamp) != 0) {
            continue; 
        }
        
        int hour = tm_buf.tm_hour;

        bool is_anomaly = false;

        // Правило 1: Слишком высокое потребление
        if (m.power_kw > config_.high_power_threshold) {
            is_anomaly = true;
        }
        // Правило 2: Слишком низкое потребление ночью
        else if (hour >= config_.night_start_hour && hour <= config_.night_end_hour &&
                 m.power_kw < config_.night_low_threshold) {
            is_anomaly = true;
        }

        if (is_anomaly) {
            m.is_anomaly = true;
            anomaly_count++;

            std::cout << std::format("АНОМАЛИЯ в {:02}:00 — {:.2f} кВт\n", 
                                     hour, m.power_kw);
        }
    }

    return anomaly_count;
}

std::vector<core::PowerMeasurement> 
AnomalyDetector::get_anomalies(const core::SimulationData& data) const
{
    std::vector<core::PowerMeasurement> anomalies;
    anomalies.reserve(data.size() / 10); 

    for (const auto& m : data) {
        if (m.is_anomaly) {
            anomalies.push_back(m);
        }
    }

    return anomalies;
}

} // namespace dorm_energy::detection