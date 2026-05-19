// src/main.cpp
#include <iostream>
// #include <locale>

#include "dorm_energy/app/application.hpp"
#include "dorm_energy/app/commands.hpp"
#include "dorm_energy/logging/logger.hpp"
#include "dorm_energy/simulation/generator.hpp"
#include "dorm_energy/detection/anomaly_detector.hpp"
#include "dorm_energy/storage/repository.hpp"
#include "dorm_energy/mqtt/mqtt_client.hpp"
#include "dorm_energy/app/message_handler.hpp"
#include "dorm_energy/app/notifier.hpp"

int main(int argc, char **argv)
{
    // std::locale::global(std::locale("ru_RU.UTF-8"));

    using namespace dorm_energy; // подумать над нужностью локализации

    try
    {
        auto root_logger = std::make_unique<logging::Logger>("dorm-energy");

        auto repository = std::make_unique<storage::MeasurementRepository>();

        auto mqtt_client = std::make_unique<mqtt::MqttClient>(
            std::make_unique<simulation::SyntheticDataGenerator>());

        auto handler = std::make_unique<app::MessageHandler>(
            std::make_unique<logging::Logger>("message-handler"),
            std::make_unique<detection::AnomalyDetector>(),
            std::make_unique<app::ConsoleNotifier>(
                std::make_unique<logging::Logger>("notifier")),
            std::make_unique<storage::MeasurementRepository>());

        auto app = std::make_unique<app::Application>(
            std::move(root_logger),
            std::move(mqtt_client),
            std::make_unique<simulation::SyntheticDataGenerator>(),
            std::make_unique<detection::AnomalyDetector>(),
            std::move(repository),
            std::move(handler));

        return app->run(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
}