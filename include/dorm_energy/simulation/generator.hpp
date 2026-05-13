// include/dorm_energy/simulation/generator.hpp
#pragma once

#include <random>
#include <vector>

#include "../core/measurement.hpp"

namespace dorm_energy::simulation
{

    /**
     * @brief Конфигурация генератора синтетических данных
     */
    struct GeneratorConfig
    {
        int days{30};
        unsigned seed{42};

        // Параметры аномалий
        bool inject_anomalies{false};
        double anomaly_rate{0.03};

        // Коэффициенты поведения
        double weekend_morning_factor{0.6};
        double weekend_evening_factor{0.7};
        double anomaly_multiplier{2.8};

        // Для распределения аномалий
        int anomaly_distribution_max{1000};

        GeneratorConfig() = default;
    };

    /**
     * @brief Генератор реалистичных данных энергопотребления общежития
     */
    class SyntheticDataGenerator
    {
    public:
        /**
         * @brief Конструктор с конфигурацией
         * @param config параметры генерации
         */
        explicit SyntheticDataGenerator(GeneratorConfig config = {});

        /**
         * @brief Генерирует данные за указанное количество дней
         * @return SimulationData — вектор измерений мощности
         */
        core::SimulationData generate() const;

        /**
         * @brief Генерирует данные
         * @param days количество дней
         * @param seed фиксированный seed
         * @return SimulationData
         */
        static core::SimulationData generate_deterministic( // почему нельзя через {}
            int days = 30,
            unsigned seed = 42,
            bool inject_anomalies = false,
            double anomaly_rate = 0.03);

    private:
        /**
         * @brief Генерирует данные за один день
         * @param day_of_week день недели (0=понедельник ... 6=воскресенье) // добавить параметр day_start
         * @param gen генератор случайных чисел
         */
        core::SimulationData generate_day(
            int day_of_week,
            core::TimePoint day_start,
            std::mt19937 &gen) const;

        GeneratorConfig config_;
    };

} // namespace dorm_energy::simulation