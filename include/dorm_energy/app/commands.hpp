// include/dorm_energy/app/commands.hpp
#pragma once

#include <CLI/CLI.hpp>
#include "../interfaces.hpp"

namespace dorm_energy::app
{

    class ICommand // вынести в интерфейс
    {
    public:
        virtual ~ICommand() = default;
        virtual int execute() = 0;
    };

    class SimulateCommand : public ICommand
    {
    public:
        SimulateCommand(
            std::unique_ptr<ILogger> logger,
            std::unique_ptr<IDataGenerator> generator,
            std::unique_ptr<IAnomalyDetector> detector,
            std::unique_ptr<IMeasurementRepository> repository,
            const CLI::App &subcommand);

        int execute() override;

    private:
        std::unique_ptr<ILogger> logger_;
        std::unique_ptr<IDataGenerator> generator_;
        std::unique_ptr<IAnomalyDetector> detector_;
        std::unique_ptr<IMeasurementRepository> repository_;
        const CLI::App &subcommand_;
    };

    class DaemonCommand : public ICommand
    {
    public:
        explicit DaemonCommand(
            std::unique_ptr<ILogger> logger,
            std::unique_ptr<IMqttClient> mqtt_client,
            std::unique_ptr<IMeasurementRepository> repository); 

        int execute() override;

    private:
        std::unique_ptr<ILogger> logger_;
        std::unique_ptr<IMqttClient> mqtt_client_;
        std::unique_ptr<IMeasurementRepository> repository_;
    };

} // namespace dorm_energy