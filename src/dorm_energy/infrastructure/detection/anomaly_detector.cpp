// src/detection/anomaly_detector.cpp
#include <format>
#include <iostream>

#include "dorm_energy/detection/anomaly_detector.hpp"

namespace dorm_energy::detection
{

    AnomalyDetector::AnomalyDetector(AnomalyDetectorConfig config)
        : config_(std::move(config))
    {
    }

    int AnomalyDetector::detect(core::SimulationData &data) const
    {
        int anomaly_count{0};

        for (auto &m : data)
        {
            if (isAnomaly(m))
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
        anomalies.reserve(data.size() / 10); // убрать маг значние

        for (const auto &m : data)
        {
            if (m.is_anomaly)
                anomalies.push_back(m);
        }

        return anomalies;
    }

    bool AnomalyDetector::isAnomaly(const core::PowerMeasurement &m) const
    {
        if (m.power_kw > config_.high_power_threshold)
            return true;

        if (m.power_kw < AnomalyDetectorConfig::night_low_threshold &&
            m.hour_of_day >= 0 && m.hour_of_day <= 6)
            return true;

        return false;
    }

} // namespace dorm_energy::detection