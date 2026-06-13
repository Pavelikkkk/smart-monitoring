// include/dorm_energy/domain/detection/ianomaly_detector.hpp
#pragma once

#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/core/measurement.hpp"
#include <string>

namespace dorm_energy::detection
{
    struct AnomalyInfo // вынести в отдельный файл
    {
        bool isAnomaly = false;
        std::string anomalyType;
        core::AlertSeverity severity = core::AlertSeverity::Warning;
        std::string description;
    };

    class IAnomalyDetector
    {
    public:
        virtual ~IAnomalyDetector() = default;

        virtual bool isAnomaly(const core::SensorReading &reading) const = 0;
        virtual std::vector<core::SensorReading>
        getAnomalies(const core::ReadingsBatch &data) const = 0;
        virtual int countAnomalies(const core::ReadingsBatch &data) const = 0;

        virtual AnomalyInfo detect(const core::SensorReading &reading) const = 0;
    };

} // namespace dorm_energy::detection