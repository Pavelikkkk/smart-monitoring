#include "dorm_energy/core/measurement.hpp"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <string>

using dorm_energy::core::SensorReading;

TEST(MeasurementTest, ZeroTimestampHasHourZero)
{
    SensorReading reading{};

    EXPECT_EQ(dorm_energy::core::getHourOfDay(reading), 0);
}

TEST(MeasurementTest, FormatsNumericReadingForLogs)
{
    SensorReading reading{};
    reading.deviceId = "room-101";
    reading.sensorType = "power";
    reading.value = 1.75;
    reading.unit = "kW";

    EXPECT_EQ(
        dorm_energy::core::toString(reading),
        "0000-00-00 00:00:00 | room-101 | power | 1.75 kW");
}

TEST(MeasurementTest, FormatsBooleanReadingForLogs)
{
    SensorReading reading{};
    reading.deviceId = "room-101";
    reading.sensorType = "motion";
    reading.value = 0.0;
    reading.boolValue = true;

    EXPECT_EQ(
        dorm_energy::core::toString(reading),
        "0000-00-00 00:00:00 | room-101 | motion | true");
}

TEST(MeasurementTest, SerializesNumericReadingToJson)
{
    SensorReading reading{};
    reading.timestamp = std::chrono::system_clock::from_time_t(1717245296);
    reading.deviceId = "room-101";
    reading.sensorType = "power";
    reading.value = 3.25;
    reading.unit = "kW";

    const auto json = nlohmann::json::parse(dorm_energy::core::toJson(reading));

    EXPECT_EQ(json.at("timestamp"), "2024-06-01T12:34:56Z");
    EXPECT_EQ(json.at("deviceId"), "room-101");
    EXPECT_EQ(json.at("sensorType"), "power");
    EXPECT_DOUBLE_EQ(json.at("value").get<double>(), 3.25);
    EXPECT_EQ(json.at("unit"), "kW");
    EXPECT_FALSE(json.contains("boolValue"));
}

TEST(MeasurementTest, SerializesBooleanReadingToJson)
{
    SensorReading reading{};
    reading.deviceId = "room-101";
    reading.sensorType = "motion";
    reading.boolValue = false;

    const auto json = nlohmann::json::parse(dorm_energy::core::toJson(reading));

    EXPECT_TRUE(json.at("timestamp").is_null());
    EXPECT_EQ(json.at("value"), false);
    EXPECT_EQ(json.at("boolValue"), false);
}
