// include/dorm_energy/infrastructure/storage/postgres_repository.hpp
#pragma once

#include "dorm_energy/domain/storage/imeasurement_repository.hpp"
#include "dorm_energy/core/measurement.hpp"
#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/domain/storage/device_dto.hpp"
#include "dorm_energy/domain/storage/building_dto.hpp"
#include "dorm_energy/domain/storage/room_dto.hpp"
#include "dorm_energy/domain/storage/power_point_dto.hpp"
#include "dorm_energy/domain/storage/energy_by_room_dto.hpp"
#include "dorm_energy/domain/storage/severity_stats_dto.hpp"

#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>

namespace dorm_energy::storage
{
    class PostgresMeasurementRepository : public IMeasurementRepository
    {
    public:
        explicit PostgresMeasurementRepository(
            const std::string &connectionString,
            std::size_t maxBufferSize = 2000);

        ~PostgresMeasurementRepository() override;

        bool save(const core::SensorReading &reading) override;
        std::size_t saveBatch(const core::ReadingsBatch &readings) override;

        bool saveAnomaly(const core::SensorReading &reading,
                         const std::string &anomalyType,
                         core::AlertSeverity severity,
                         const std::string &description,
                         double score) override;

        void flush() override;

        std::vector<AnomalyDto>
        getLatestAnomalies(
            std::size_t limit = 20) override;

        std::vector<PowerPointDto>
        getPowerHistory(
            int hours = 24) override;

        std::vector<DeviceDto>
        getDevices() override;

        std::vector<BuildingDto>
        getBuildings() override;

        std::vector<RoomDto>
        getRooms() override;

        std::vector<TopConsumerDto>
        getTopConsumers(
            int limit = 10) override;

        std::vector<AnomalyStatsDto>
        getAnomalyStatistics() override;

        std::vector<EnergyByRoomDto>
        getEnergyByRoom() override;

        std::vector<SeverityStatsDto>
        getSeverityDistribution() override;

    private:
        void connect();
        bool tryReconnect(int maxAttempts = 3);

        void doFlush(const std::vector<core::SensorReading> &readings);

        std::string connectionString_;
        std::unique_ptr<pqxx::connection> connection_;

        std::vector<core::SensorReading> buffer_;
        const std::size_t maxBufferSize_;

        std::mutex bufferMutex_; // thread-safety для будущего MQTT
    };
} // namespace dorm_energy::storage