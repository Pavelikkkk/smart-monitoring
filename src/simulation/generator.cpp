// src\simulation\generator.cpp
#include <chrono>
#include <random>

#include "dorm_energy/simulation/generator.hpp"

namespace dorm_energy::simulation
{

    SyntheticDataGenerator::SyntheticDataGenerator(GeneratorConfig config)
        : config_{std::move(config)}
    {
        if (config_.days < 1 || config_.days > 365)
        {
            config_.days = 30;
        }
    }

    core::SimulationData SyntheticDataGenerator::generate() const
    {
        return generate_deterministic(
            config_.days,
            config_.seed,
            config_.inject_anomalies,
            config_.anomaly_rate);
    }

    core::SimulationData SyntheticDataGenerator::generate_deterministic(
        int days,
        unsigned seed,
        bool inject_anomalies,
        double anomaly_rate)
    {
        core::SimulationData data;
        data.reserve(static_cast<size_t>(days) * 24); // часы в сутках

        std::mt19937 gen{seed};

        const auto base_time = std::chrono::sys_days{
            std::chrono::year{2025} / std::chrono::January / 1}; // что такое 1

        const std::chrono::weekday start_dow{base_time};

        GeneratorConfig temp_config;
        temp_config.days = days;
        temp_config.seed = seed;
        temp_config.inject_anomalies = inject_anomalies;
        temp_config.anomaly_rate = anomaly_rate;

        SyntheticDataGenerator temp_gen{std::move(temp_config)};

        for (int d = 0; d < days; ++d)
        {
            const int day_of_week = (start_dow + std::chrono::days{d}).c_encoding();
            const auto day_start = base_time + std::chrono::days{d};

            auto day_data = temp_gen.generate_day(day_of_week, day_start, gen);

            data.insert(data.end(),
                        std::make_move_iterator(day_data.begin()),
                        std::make_move_iterator(day_data.end()));
        }

        return data;
    }

    core::SimulationData SyntheticDataGenerator::generate_day(
        int day_of_week,
        core::TimePoint day_start,
        std::mt19937 &gen) const
    {
        core::SimulationData day_data;
        day_data.reserve(24);

        double night_base = 5.0; // {}
        double morning_peak = 25.0;
        double day_base = 12.0;
        double evening_peak = 35.0;

        if (day_of_week >= 5)
        {
            morning_peak *= config_.weekend_morning_factor;
            evening_peak *= config_.weekend_evening_factor;
        }

        std::normal_distribution<double> noise{0.0, 2.5};
        std::uniform_int_distribution<int> anomaly_dist{0, config_.anomaly_distribution_max - 1};

        for (int hour = 0; hour < 24; ++hour)
        {
            double base = night_base;
            if (hour >= 7 && hour < 10)
                base = morning_peak;
            else if (hour >= 10 && hour < 17)
                base = day_base;
            else if (hour >= 17 && hour < 23)
                base = evening_peak;

            double power = base + noise(gen);
            if (power < 0.0)
                power = 0.0;

            bool is_anomaly = false;
            if (config_.inject_anomalies &&
                (static_cast<double>(anomaly_dist(gen)) / config_.anomaly_distribution_max < config_.anomaly_rate))
            {
                power *= config_.anomaly_multiplier;
                is_anomaly = true;
            }

            auto current_time = day_start + std::chrono::hours(hour);

            day_data.emplace_back(current_time, power, hour, is_anomaly);
        }

        return day_data;
    }

} // namespace dorm_energy::simulation