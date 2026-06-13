#include "dorm_energy/infrastructure/detection/feature_extractor.hpp"

#include <cmath>
#include <ctime>
#include <numbers>

namespace dorm_energy::detection
{

    std::array<float, 5> FeatureExtractor::extract(
        const DetectionContext &context)
    {
        auto timestamp = context.current.timestamp;
        auto time = std::chrono::system_clock::to_time_t(timestamp);
        std::tm tm{};

#ifdef _WIN32
        localtime_s(
            &tm,
            &time);
#else
        localtime_r(
            &time,
            &tm);
#endif

        int hour = tm.tm_hour;

        float hourSin = std::sin(2.0f * std::numbers::pi_v<float> * hour / 24.0f);

        float hourCos = std::cos(2.0f * std::numbers::pi_v<float> * hour / 24.0f);

        return {
            context.current.motion ? 1.f : 0.f,
            static_cast<float>(context.current.power),
            static_cast<float>(context.current.light),
            hourSin,
            hourCos};
    }

}
