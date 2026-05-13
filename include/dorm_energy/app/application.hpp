// include/dorm_energy/app/application.hpp
#pragma once

#include <CLI/CLI.hpp>
#include <atomic>
#include <csignal>

#include "../simulation/generator.hpp"
#include "../detection/anomaly_detector.hpp"
#include "../logging/logger.hpp"
#include "../mqtt/mqtt_client.hpp"
#include "../storage/repository.hpp"

namespace dorm_energy
{

    class Application
    {
    public:
        explicit Application(logging::Logger logger = logging::Logger{},
                             mqtt::MqttClient mqtt_client = {});

        ~Application() = default;

        int run(int argc, char *argv[]);

        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

    private:
        int handle_simulate(const CLI::App &subcommand);
        int handle_daemon();

        logging::Logger logger_;
        mqtt::MqttClient mqtt_client_;
        simulation::SyntheticDataGenerator generator_;
        detection::AnomalyDetector detector_;
        storage::MeasurementRepository repository_;

        static inline std::atomic<bool> running_{true};
    };

} // namespace dorm_energy