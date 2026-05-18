// src/main.cpp
#include "dorm_energy/app/application.hpp"
#include "dorm_energy/logging/logger.hpp"
#include "dorm_energy/mqtt/mqtt_client.hpp"
#include "dorm_energy/simulation/generator.hpp"
#include "dorm_energy/detection/anomaly_detector.hpp"
#include "dorm_energy/storage/repository.hpp"

int main(int argc, char** argv)
{
    using namespace dorm_energy;

    auto app = Application(
        std::make_unique<logging::Logger>("dorm-energy"),
        std::make_unique<mqtt::MqttClient>(),
        std::make_unique<simulation::SyntheticDataGenerator>(),
        std::make_unique<detection::AnomalyDetector>(),
        std::make_unique<storage::MeasurementRepository>()
    );

    return app.run(argc, argv);
}