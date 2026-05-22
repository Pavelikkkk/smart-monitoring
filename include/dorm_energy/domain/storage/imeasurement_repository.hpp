// include/dorm_energy/domain/storage/imeasurement_repository.hpp
#pragma once

#include "dorm_energy/core/measurement.hpp"

namespace dorm_energy::storage
{

    /**
     * @brief Интерфейс для сохранения показаний датчиков
     *
     * Реализации: PostgreSQL (TimescaleDB)
     */
    class IMeasurementRepository
    {
    public:
        virtual ~IMeasurementRepository() = default;

        /**
         * @brief Сохраняет одно показание
         */
        virtual void save(const core::SensorReading &reading) = 0;

        /**
         * @brief Сохраняет пакет показаний
         */
        virtual void saveBatch(const core::ReadingsBatch &readings) = 0;

        // virtual ReadingsBatch find_by_device(const std::string& device_id, std::size_t limit) const = 0;
        // virtual std::size_t count() const = 0;
    };

} // namespace dorm_energy::domain::storage