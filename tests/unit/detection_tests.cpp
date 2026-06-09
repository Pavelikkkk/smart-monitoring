#include "dorm_energy/infrastructure/detection/anomaly_tracker.hpp"
#include "dorm_energy/infrastructure/detection/feature_extractor.hpp"
#include "dorm_energy/infrastructure/detection/room_state_aggregator.hpp"
#include "dorm_energy/infrastructure/detection/rule_based_detector.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <ctime>
#include <deque>

using dorm_energy::core::AlertSeverity;
using dorm_energy::core::RoomState;
using dorm_energy::core::SensorReading;
using dorm_energy::detection::AnomalyInfo;
using dorm_energy::detection::DetectionContext;
using dorm_energy::detection::RuleBasedDetector;

namespace
{
    std::chrono::system_clock::time_point localTime(int hour)
    {
        std::tm tm{};
        tm.tm_year = 124;
        tm.tm_mon = 0;
        tm.tm_mday = 2;
        tm.tm_hour = hour;
        tm.tm_isdst = -1;
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }
}

TEST(RuleBasedDetectorTest, DetectsHighPower)
{
    RuleBasedDetector detector(5.0);
    DetectionContext context{};
    context.current.power = 5.01;

    const auto result = detector.detect(context);

    EXPECT_TRUE(result.isAnomaly);
    EXPECT_EQ(result.anomalyType, "rule_high_power");
    EXPECT_EQ(result.severity, AlertSeverity::Warning);
    EXPECT_TRUE(detector.isAnomaly(context));
}

TEST(RuleBasedDetectorTest, DetectsLightWithoutMotion)
{
    RuleBasedDetector detector(10.0);
    DetectionContext context{};
    context.current.motion = false;
    context.current.light = 501.0;

    const auto result = detector.detect(context);

    EXPECT_TRUE(result.isAnomaly);
    EXPECT_EQ(result.anomalyType, "rule_light_without_motion");
    EXPECT_EQ(result.severity, AlertSeverity::Info);
}

TEST(RuleBasedDetectorTest, DetectsPowerWithoutMotionForThirtyMinutes)
{
    RuleBasedDetector detector(10.0);
    const auto start = std::chrono::system_clock::from_time_t(1717243200);

    std::deque<RoomState> history{
        RoomState{.deviceId = "room-101", .timestamp = start, .motion = false},
        RoomState{.deviceId = "room-101", .timestamp = start + std::chrono::minutes(30), .motion = false},
    };

    DetectionContext context{};
    context.current = history.back();
    context.current.power = 6.0;
    context.history = &history;

    const auto result = detector.detect(context);

    EXPECT_TRUE(result.isAnomaly);
    EXPECT_EQ(result.anomalyType, "rule_power_without_motion_30m");
}

TEST(RuleBasedDetectorTest, IgnoresNormalStateAndInvalidThresholdFallsBack)
{
    RuleBasedDetector detector(-1.0);
    DetectionContext context{};
    context.current.power = 6.0;
    context.current.motion = true;
    context.current.light = 250.0;

    EXPECT_FALSE(detector.detect(context).isAnomaly);
}

TEST(RoomStateAggregatorTest, AggregatesReadingsAndKeepsThirtyMinuteHistory)
{
    dorm_energy::detection::RoomStateAggregator aggregator;
    const auto start = std::chrono::system_clock::from_time_t(1717243200);

    aggregator.update(SensorReading{.timestamp = start, .deviceId = "room-101", .sensorType = "motion", .value = 0.0, .boolValue = true});
    aggregator.update(SensorReading{.timestamp = start + std::chrono::minutes(10), .deviceId = "room-101", .sensorType = "power", .value = 2.25});
    const auto current = aggregator.update(SensorReading{.timestamp = start + std::chrono::minutes(31), .deviceId = "room-101", .sensorType = "light", .value = 700.0});

    ASSERT_TRUE(current.has_value());
    EXPECT_EQ(current->deviceId, "room-101");
    EXPECT_TRUE(current->motion);
    EXPECT_DOUBLE_EQ(current->power, 2.25);
    EXPECT_DOUBLE_EQ(current->light, 700.0);

    EXPECT_EQ(aggregator.getCurrentStates().size(), 1U);
    EXPECT_EQ(aggregator.getHistory("room-101").size(), 2U);
    EXPECT_TRUE(aggregator.getHistory("missing").empty());
}

TEST(AnomalyTrackerTest, SuppressesDuplicatesUntilRoomIsResolved)
{
    dorm_energy::detection::AnomalyTracker tracker;
    RoomState state{};
    state.deviceId = "room-101";

    AnomalyInfo anomaly{};
    anomaly.anomalyType = "rule_high_power";

    EXPECT_TRUE(tracker.shouldReport(state, anomaly));
    EXPECT_FALSE(tracker.shouldReport(state, anomaly));

    tracker.resolveRoom("room-101");

    EXPECT_TRUE(tracker.shouldReport(state, anomaly));
}

TEST(FeatureExtractorTest, ExtractsMotionPowerLightAndHourFeatures)
{
    DetectionContext context{};
    context.current.motion = true;
    context.current.power = 3.5;
    context.current.light = 400.0;
    context.current.timestamp = localTime(6);

    const auto features = dorm_energy::detection::FeatureExtractor::extract(context);

    EXPECT_FLOAT_EQ(features[0], 1.0F);
    EXPECT_FLOAT_EQ(features[1], 3.5F);
    EXPECT_FLOAT_EQ(features[2], 400.0F);
    EXPECT_NEAR(features[3], 1.0F, 0.0001F);
    EXPECT_NEAR(features[4], 0.0F, 0.0001F);
}
