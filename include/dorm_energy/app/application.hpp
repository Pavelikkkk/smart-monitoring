// include/dorm_energy/app/application.hpp
#pragma once

#include <CLI/CLI.hpp>

#include "../interfaces.hpp"
#include "commands.hpp"

namespace dorm_energy
{
    class Application
    {
    public:
        explicit Application(
            std::unique_ptr<ILogger> logger,
            std::unique_ptr<IMqttClient> mqtt_client,
            std::unique_ptr<IDataGenerator> generator,
            std::unique_ptr<IAnomalyDetector> detector,
            std::unique_ptr<IMeasurementRepository> repository);

        int run(int argc, char **argv);

        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

    private:
        std::unique_ptr<ILogger> logger_;
        std::unique_ptr<IMqttClient> mqtt_client_;
        std::unique_ptr<IDataGenerator> generator_;
        std::unique_ptr<IAnomalyDetector> detector_;
        std::unique_ptr<IMeasurementRepository> repository_;
    };

} // namespace dorm_energy