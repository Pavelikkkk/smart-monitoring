// include/dorm_energy/application/commands/simulate_command.hpp
#pragma once

#include "dorm_energy/application/cli/command_options.hpp"

#include "dorm_energy/domain/logging/ilogger.hpp"
#include "dorm_energy/domain/simulation/idata_generator.hpp"
#include "dorm_energy/domain/detection/ianomaly_detector.hpp"
#include "dorm_energy/domain/storage/imeasurement_repository.hpp"

#include <memory>

namespace dorm_energy::application
{

    /**
     * @brief Команда симуляции генерации данныё
     */
    class SimulateCommand
    {
    public:
        explicit SimulateCommand(
            std::unique_ptr<domain::logging::ILogger> logger,
            std::unique_ptr<domain::simulation::IDataGenerator> generator,
            std::unique_ptr<domain::detection::IAnomalyDetector> detector,
            std::unique_ptr<domain::storage::IMeasurementRepository> repository);

        /**
         * @brief Выполняет симуляцию
         * @param options параметры из командной строки
         * @return код завершения
         */
        int execute(const cli::CommandOptions &options);

    private:
        std::unique_ptr<domain::logging::ILogger> logger_;
        std::unique_ptr<domain::simulation::IDataGenerator> generator_;
        std::unique_ptr<domain::detection::IAnomalyDetector> detector_;
        std::unique_ptr<domain::storage::IMeasurementRepository> repository_;
    };

} // namespace dorm_energy::application