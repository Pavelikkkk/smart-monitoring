// src/storage/repository.cpp

#include <iostream>
#include <format>

#include "dorm_energy/interfaces.hpp"
#include "dorm_energy/storage/repository.hpp" // почему так а не просто repository.hpp

namespace dorm_energy::storage
{

    MeasurementRepository::MeasurementRepository(StorageConfig config)
        : config_(std::move(config)) {}

    MeasurementRepository::~MeasurementRepository()
    {
        // disconnect
    }

    bool MeasurementRepository::connect() // методы bool или void и почему
    {
        std::cout << "[DB] Подключение к PostgreSQL/TimescaleDB...\n";
        // TODO: pqxx::connection
        connected_ = true;
        std::cout << "[DB] ✅ Подключено успешно (заглушка)\n";
        return true;
    }

    void MeasurementRepository::save(const core::PowerMeasurement &m)
    {
        if (!connected_)
            return;

        std::cout << std::format("[DB] Сохранено: {} кВт в {}:00{}\n",
                                 m.power_kw,
                                 m.hour_of_day,
                                 m.is_anomaly ? " (АНОМАЛИЯ)" : "");
    }

    void MeasurementRepository::save_batch(const core::SimulationData &data)
    {
        std::cout << std::format(
            "[DB] Сохранена пачка из {} измерений\n",
            data.size());

        for (const auto &m : data)
        {
            save(m);
        }
    }
} // namespace dorm_energy::storage