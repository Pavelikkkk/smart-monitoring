// src/simulation/generator.cpp
#include "dorm_energy/simulation/generator.hpp"
#include <format>
#include <iostream>

namespace dorm_energy::simulation {

SyntheticDataGenerator::SyntheticDataGenerator(GeneratorConfig config)
    : config_{std::move(config)}
{
    if (config_.days < 1 || config_.days > 365) {
        config_.days = 30; 
    }
}

core::SimulationData SyntheticDataGenerator::generate() const
{
    std::mt19937 gen(config_.seed);
    return generate_deterministic(config_.days, config_.seed);
}

core::SimulationData SyntheticDataGenerator::generate_deterministic(int days, unsigned seed)
{
    std::mt19937 gen(seed);
    core::SimulationData all_data;
    all_data.reserve(static_cast<size_t>(days) * 24);

    for (int day = 0; day < days; ++day) {
        int day_of_week = day % 7;
        auto day_data = SyntheticDataGenerator{}.generate_day(day_of_week, gen); 
        all_data.insert(all_data.end(),
                        std::make_move_iterator(day_data.begin()),
                        std::make_move_iterator(day_data.end()));
    }

    return all_data;
}

core::SimulationData SyntheticDataGenerator::generate_day(int day_of_week, std::mt19937& gen) const
{
    core::SimulationData day_data;
    day_data.reserve(24);

    double night_base   = 5.0;
    double morning_peak = 25.0;
    double day_base     = 12.0;
    double evening_peak = 35.0;

    if (day_of_week >= 5) {  
        morning_peak *= 0.6;
        evening_peak *= 0.7;
    }

    std::normal_distribution<double> noise(0.0, 2.5);

    std::time_t now = std::time(nullptr);

    for (int hour = 0; hour < 24; ++hour) {
        double base = night_base;
        if (hour >= 7 && hour < 10)      base = morning_peak;
        else if (hour >= 10 && hour < 17) base = day_base;
        else if (hour >= 17 && hour < 23) base = evening_peak;

        double power = base + noise(gen);
        if (power < 0.0) power = 0.0;

        bool is_anomaly = false;
        if (config_.inject_anomalies && 
            (static_cast<double>(std::uniform_int_distribution<int>(0, 999)(gen)) / 1000.0 < config_.anomaly_rate)) {
            power *= 2.8;  
            is_anomaly = true;
        }

        std::time_t timestamp = now - (23 - hour + day_of_week * 24LL * 3600) * 3600LL; 

        day_data.emplace_back(timestamp, power, is_anomaly);
    }

    return day_data;
}

} // namespace dorm_energy::simulation