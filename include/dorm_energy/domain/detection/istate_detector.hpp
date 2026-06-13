#pragma once

#include "dorm_energy/core/detection_context.hpp"
#include "dorm_energy/domain/detection/anomaly_info.hpp"

namespace dorm_energy::detection
{

    class IStateDetector
    {
    public:
        virtual ~IStateDetector() = default;

        virtual bool isAnomaly(const DetectionContext &context) const = 0;

        virtual AnomalyInfo detect(const DetectionContext &context) const = 0;
    };

} // namespace dorm_energy::detection