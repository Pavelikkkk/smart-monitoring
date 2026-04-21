// include/dorm_energy/simulation/generator.hpp
#pragma once

#include "../core/measurement.hpp"
#include <random>
#include <vector>

namespace dorm_energy::simulation {

/**
 * @brief Конфигурация генератора синтетических данных
 */
struct GeneratorConfig {
    int         days = 30;                    
    unsigned    seed = std::random_device{}(); 
    bool        inject_anomalies = false;     
    double      anomaly_rate = 0.03;          

    GeneratorConfig() = default;
};

/**
 * @brief Генератор реалистичных данных энергопотребления общежития
 */
class SyntheticDataGenerator {
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
     * @brief Генерирует полностью детерминированные данные 
     * @param days количество дней
     * @param seed фиксированный seed
     * @return SimulationData
     */
    static core::SimulationData generate_deterministic(int days, unsigned seed = 42);

private:
    /**
     * @brief Генерирует данные за один день
     * @param day_of_week день недели (0=понедельник ... 6=воскресенье)
     * @param gen генератор случайных чисел
     */
    core::SimulationData generate_day(int day_of_week, std::mt19937& gen) const;

    GeneratorConfig config_;
};

} // namespace dorm_energy::simulation