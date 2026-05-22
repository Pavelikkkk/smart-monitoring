// include/dorm_energy/domain/simulation/idata_generator.hpp
#pragma once

#include "dorm_energy/core/measurement.hpp"

namespace dorm_energy::simulation
{
    /**
     * @brief Интерфейс для генерации синтетических данных
     *
     * Используется в симуляции и тестах.
     */
    class IDataGenerator
    {
    public:
        virtual ~IDataGenerator() = default;

        /**
         * @brief Генерирует батч показаний датчиков
         * @return Набор сгенерированных SensorReading
         */
        virtual core::ReadingsBatch generate() const = 0;
    };

} // namespace dorm_energy::domain::simulation