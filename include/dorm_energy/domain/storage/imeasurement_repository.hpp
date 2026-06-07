// include/dorm_energy/domain/storage/imeasurement_repository.hpp
#pragma once

#include "dorm_energy/core/measurement.hpp"
#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/domain/storage/anomaly_dto.hpp"
#include "dorm_energy/domain/storage/power_point_dto.hpp"
#include "dorm_energy/domain/storage/device_dto.hpp"
#include "dorm_energy/domain/storage/building_dto.hpp"
#include "dorm_energy/domain/storage/room_dto.hpp"
#include "dorm_energy/domain/storage/top_consumer_dto.hpp"
#include "dorm_energy/domain/storage/anomaly_stats_dto.hpp"
#include "dorm_energy/domain/storage/energy_by_room_dto.hpp"
#include "dorm_energy/domain/storage/severity_stats_dto.hpp"

namespace dorm_energy::storage
{
    /**
     * @brief Интерфейс для сохранения показаний датчиков
     */
    class IMeasurementRepository
    {
    public:
        virtual ~IMeasurementRepository() = default;

        virtual bool save(const core::SensorReading &reading) = 0; // TODO: переименовать в saveReading
        virtual std::size_t saveBatch(const core::ReadingsBatch &readings) = 0;

        virtual void flush() = 0;

        virtual bool saveAnomaly(const core::SensorReading &reading,
                                 const std::string &anomalyType,
                                 core::AlertSeverity severity,
                                 const std::string &description,
                                 double score = 0.0) = 0;

        virtual std::vector<AnomalyDto>
        getLatestAnomalies(
            std::size_t limit = 20) = 0;

        virtual std::vector<PowerPointDto>
        getPowerHistory(
            int hours = 24) = 0;

        virtual std::vector<DeviceDto>
        getDevices() = 0;
        
        virtual std::vector<BuildingDto>
        getBuildings() = 0;

        virtual std::vector<RoomDto>
        getRooms() = 0;

        virtual std::vector<TopConsumerDto>
        getTopConsumers(
            int limit = 10) = 0;

        virtual std::vector<AnomalyStatsDto>
        getAnomalyStatistics() = 0;

        virtual std::vector<EnergyByRoomDto>
        getEnergyByRoom() = 0;

        virtual std::vector<SeverityStatsDto>
        getSeverityDistribution() = 0;
        // virtual void shutdown() = 0;
    };
} // namespace dorm_energy::storage