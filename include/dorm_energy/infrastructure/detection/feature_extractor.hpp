#pragma once

#include "dorm_energy/core/detection_context.hpp"

#include <array>

namespace dorm_energy::detection
{

    class FeatureExtractor
    {
    public:
        static std::array<float, 5> extract(const DetectionContext &context);
    };

} // namespace dorm_energy::detection