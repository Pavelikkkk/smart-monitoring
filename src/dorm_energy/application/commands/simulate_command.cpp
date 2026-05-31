// src/dorm_energy/application/commands/simulate_command.cpp
#include "dorm_energy/application/commands/simulate_command.hpp"
#include "dorm_energy/core/measurement.hpp"
#include "dorm_energy/domain/detection/ianomaly_detector.hpp"
#include "dorm_energy/domain/simulation/idata_generator.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/domain/logging/ilogger.hpp"
#include "dorm_energy/infrastructure/simulation/csv_exporter.hpp"

#include <fmt/format.h>
#include <iostream>

namespace dorm_energy::application
{

    SimulateCommand::SimulateCommand(
        std::shared_ptr<dorm_energy::logging::ILogger> logger,
        std::unique_ptr<dorm_energy::simulation::IDataGenerator> generator,
        std::unique_ptr<dorm_energy::detection::IAnomalyDetector> detector,
        std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository)
        : logger_(std::move(logger)),
          generator_(std::move(generator)),
          detector_(std::move(detector)),
          repository_(std::move(repository))
    {
        if (!logger_ || !generator_ || !detector_ || !repository_)
        {
            throw std::invalid_argument("SimulateCommand: all dependencies must be provided");
        }
    }

    int SimulateCommand::execute(const cli::CommandOptions &options)
    {
        logger_->info("Starting simulation for " + std::to_string(options.simulateDays) + " days");

        if (options.injectAnomalies)
        {
            logger_->info("Anomaly injection mode enabled");
        }

        core::ReadingsBatch batch = generator_->generate_for_days(options.simulateDays);

        logger_->info("Generated " + std::to_string(batch.size()) + " sensor readings");

        simulation::CsvExporter::exportReadings(batch, "../../data/training_dataset.csv"); // вынести в конфиг путь изменение пути

        int anomalyCount = detector_->countAnomalies(batch);
        logger_->info("Detected anomalies: " + std::to_string(anomalyCount));

        std::size_t saved = repository_->saveBatch(batch);
        logger_->info("Saved " + std::to_string(saved) + " readings to the database");

        if (anomalyCount > 0)
        {
            auto anomalies = detector_->getAnomalies(batch);
            std::cout << "\n=== ANOMALIES DETECTED (" << anomalyCount << ") ===\n";

            int savedAnomalies = 0;

            for (const auto &reading : anomalies)
            {
                auto info = detector_->detect(reading);

                bool success = repository_->saveAnomaly(
                    reading,
                    info.anomalyType,
                    info.severity,
                    info.description);

                if (success)
                    ++savedAnomalies;

                std::cout << core::toString(reading)
                          << " → " << info.anomalyType
                          << " (" << core::toString(info.severity) << ")\n";
            }

            logger_->info("Saved " + std::to_string(savedAnomalies) + " anomalies to anomalies table");
        }

        logger_->info("Simulation completed successfully");
        return 0;
    }

} // namespace dorm_energy::application