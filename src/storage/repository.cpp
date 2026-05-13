#include "dorm_energy/storage/repository.hpp"
#include <iostream>
#include <format>

namespace dorm_energy::storage {

MeasurementRepository::MeasurementRepository(StorageConfig config)
    : config_(std::move(config)) {}

MeasurementRepository::~MeasurementRepository() {
    // disconnect
}

bool MeasurementRepository::connect() {
    std::cout << "[DB] Подключение к PostgreSQL/TimescaleDB...\n";
    // TODO: pqxx::connection
    connected_ = true;
    std::cout << "[DB] ✅ Подключено успешно (заглушка)\n";
    return true;
}

bool MeasurementRepository::save(const core::PowerMeasurement& m) {
    if (!connected_) return false;

    std::cout << std::format("[DB] Сохранено: {} кВт в {}:00{}\n", 
                             m.power_kw, m.hour_of_day, 
                             m.is_anomaly ? " (АНОМАЛИЯ)" : "");
    return true;
}

bool MeasurementRepository::save_batch(const core::SimulationData& data) {
    std::cout << std::format("[DB] Сохранена пачка из {} измерений\n", data.size());
    for (const auto& m : data) {
        save(m);
    }
    return true;
}

} // namespace dorm_energy::storage