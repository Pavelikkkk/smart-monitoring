#include <format>

#include "dorm_energy/storage/repository.hpp"
#include "dorm_energy/logging/logger.hpp"

namespace dorm_energy::storage
{

    MeasurementRepository::MeasurementRepository(StorageConfig config,
                                                 std::unique_ptr<ILogger> logger)
        : config_(std::move(config)), logger_(std::move(logger))
    {
        if (!logger_)
            logger_ = std::make_unique<logging::Logger>("repository");

        logger_->info("MeasurementRepository инициализирован (режим заглушки)");
    }

    MeasurementRepository::~MeasurementRepository() = default;

    void MeasurementRepository::save(const core::PowerMeasurement &measurement)
    {
        logger_->debug(std::format("Сохранено в БД: {:.2f} кВт (час {:02d})",
                                   measurement.power_kw, measurement.hour_of_day));
    }

    void MeasurementRepository::save_batch(const core::SimulationData &data)
    {
        if (data.empty())
            return;

        logger_->info(std::format("Сохранено батчем в БД: {} измерений", data.size()));
        // TODO: позже TimescaleDB
    }

} // namespace dorm_energy::storage