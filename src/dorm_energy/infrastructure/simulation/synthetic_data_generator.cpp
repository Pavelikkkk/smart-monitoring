// src/dorm_energy/infrastructure/simulation/synthetic_data_generator.cpp
#include "dorm_energy/infrastructure/simulation/synthetic_data_generator.hpp"
#include "dorm_energy/core/measurement.hpp"
#include "dorm_energy/domain/storage/device_dto.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"

#include <chrono>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>
#include <iostream>

namespace dorm_energy::simulation
{

    SyntheticDataGenerator::SyntheticDataGenerator::SyntheticDataGenerator(
        unsigned seed,
        bool inject_anomalies,
        double anomaly_rate,
        std::shared_ptr<storage::IMeasurementRepository> repository)
        : rng_(seed),
          inject_anomalies_(inject_anomalies),
          anomaly_rate_(anomaly_rate),
          repository_(repository)
    {
    }
    void SyntheticDataGenerator::setSeed(unsigned seed)
    {
        rng_.seed(seed);
    }
    SyntheticDataGenerator::RoomState
    SyntheticDataGenerator::generateRoomState(
        const std::string &deviceId,
        std::chrono::system_clock::time_point timestamp) const
    {
        const int hour = extract_hour(timestamp);

        const bool isNight = hour < 6;
        const bool isMorning = hour >= 6 && hour < 10;
        const bool isDay = hour >= 10 && hour < 18;

        double motionProbability;

        if (isNight)
            motionProbability = 0.02;
        else if (isMorning)
            motionProbability = 0.75;
        else if (isDay)
            motionProbability = 0.45;
        else
            motionProbability = 0.65;

        std::bernoulli_distribution motionDist(
            motionProbability);

        bool motion = motionDist(rng_);

        double power;
        double light;

        if (motion)
        {
            power =
                std::uniform_real_distribution<double>(
                    2.5,
                    6.5)(rng_);

            light =
                std::uniform_real_distribution<double>(
                    300.0,
                    800.0)(rng_);
        }
        else
        {
            power =
                std::uniform_real_distribution<double>(
                    0.1,
                    0.8)(rng_);

            light =
                std::uniform_real_distribution<double>(
                    0.0,
                    30.0)(rng_);
        }

        if (isNight)
        {
            power *= 0.7;

            if (!motion)
                light *= 0.3;
        }
        else if (isMorning)
        {
            power *= 1.1;
        }
        else if (isDay)
        {
            power *= 1.0;
        }
        else
        {
            power *= 1.2;
        }

        if (inject_anomalies_ &&
            std::uniform_real_distribution<double>(
                0.0,
                1.0)(rng_) < anomaly_rate_)
        {
            int anomalyType =
                std::uniform_int_distribution<int>(
                    0,
                    2)(rng_);

            switch (anomalyType)
            {
            case 0:
                motion = false;
                power = std::uniform_real_distribution<double>(
                    5.0,
                    10.0)(rng_);
                break;

            case 1:
                motion = false;
                light = std::uniform_real_distribution<double>(
                    500.0,
                    900.0)(rng_);
                break;

            case 2:
                motion = false;
                power = std::uniform_real_distribution<double>(
                    5.0,
                    10.0)(rng_);

                light = std::uniform_real_distribution<double>(
                    500.0,
                    900.0)(rng_);
                break;
            }
        }

        return RoomState{
            motion,
            power,
            light,
            deviceId,
            timestamp};
    }

    core::ReadingsBatch SyntheticDataGenerator::generate() const
    {
        return generate_for_days(1);
    }

    core::ReadingsBatch
    SyntheticDataGenerator::generate_for_days(
        int days) const
    {
        if (days <= 0)
            days = 1;

        core::ReadingsBatch batch;

        auto devices =
            repository_->getDevices();

        if (devices.empty())
        {
            std::cout
                << "[Generator] No devices found in database\n";

            return batch;
        }

        auto now =
            std::chrono::system_clock::now();

        auto start =
            now - std::chrono::hours(24 * days);

        for (int day = 0; day < days; ++day)
        {
            for (int hour = 0; hour < 24; ++hour)
            {
                for (const auto &device : devices)
                {
                    for (int sample = 0; sample < 6; ++sample)
                    {
                        auto timestamp =
                            start +
                            std::chrono::hours(day * 24 + hour) +
                            std::chrono::minutes(sample * 10);

                        auto state =
                            generateRoomState(
                                device.deviceId,
                                timestamp);

                        // Motion

                        core::SensorReading motion;

                        motion.timestamp =
                            timestamp;

                        motion.deviceId =
                            device.deviceId;

                        motion.sensorType =
                            "motion";

                        motion.boolValue =
                            state.motion;

                        motion.unit =
                            "bool";

                        batch.push_back(
                            motion);

                        // Power

                        core::SensorReading power;

                        power.timestamp =
                            timestamp;

                        power.deviceId =
                            device.deviceId;

                        power.sensorType =
                            "power";

                        power.value =
                            state.power;

                        power.unit =
                            "kW";

                        batch.push_back(
                            power);

                        // Light

                        core::SensorReading light;

                        light.timestamp =
                            timestamp;

                        light.deviceId =
                            device.deviceId;

                        light.sensorType =
                            "light";

                        light.value =
                            state.light;

                        light.unit =
                            "lux";

                        batch.push_back(
                            light);
                    }
                }
            }
        }

        std::cout
            << "[Generator] Generated "
            << batch.size()
            << " readings for "
            << devices.size()
            << " devices\n";

        return batch;
    }

    int SyntheticDataGenerator::extract_hour(
        std::chrono::system_clock::time_point tp) const
    {
        auto tt = std::chrono::system_clock::to_time_t(tp);

        std::tm localTime{};

#ifdef _WIN32
        localtime_s(&localTime, &tt);
#else
        localtime_r(&tt, &localTime);
#endif

        return localTime.tm_hour;
    }

} // namespace dorm_energy::simulation