// include/dorm_energy/infrastructure/detection/rule_based_detector.hpp
#pragma once

#include "dorm_energy/core/detection_context.hpp"
#include "dorm_energy/domain/detection/istate_detector.hpp"

#include <deque>

namespace dorm_energy::detection
{
    class RuleBasedDetector : public IStateDetector
    {
    public:
        explicit RuleBasedDetector(double maxPowerKw = 5.0);

        bool isAnomaly(const DetectionContext &context) const override;

        AnomalyInfo detect(const DetectionContext &context) const override;

    private:
        double maxPowerKw_;

        bool noMotionFor30Minutes(const std::deque<core::RoomState> &history) const;
    };

} // namespace dorm_energy::detection