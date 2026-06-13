// include/dorm_energy/application/commands/simulate_command.hpp
#pragma once

#include "dorm_energy/application/cli/command_options.hpp"

#include "dorm_energy/domain/detection/istate_detector.hpp"
#include "dorm_energy/domain/logging/ilogger.hpp"
#include "dorm_energy/domain/simulation/idata_generator.hpp"
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
            std::shared_ptr<dorm_energy::logging::ILogger> logger,
            std::unique_ptr<dorm_energy::simulation::IDataGenerator> generator,
            std::unique_ptr<dorm_energy::detection::IStateDetector> detector,
            std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository);

        /**
         * @brief Выполняет симуляцию
         * @param options параметры из командной строки
         * @return код завершения
         */
        int execute(const cli::CommandOptions &options);

    private:
        std::shared_ptr<dorm_energy::logging::ILogger> logger_;
        std::unique_ptr<dorm_energy::simulation::IDataGenerator> generator_;
        std::unique_ptr<dorm_energy::detection::IStateDetector> detector_;
        std::shared_ptr<dorm_energy::storage::IMeasurementRepository> repository_;
    };

} // namespace dorm_energy::application