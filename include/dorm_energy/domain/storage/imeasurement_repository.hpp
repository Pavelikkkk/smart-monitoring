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
#include "dorm_energy/domain/storage/user_dto.hpp"

#include <json/json.h>
#include <optional>

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
            std::size_t limit = 20,
            int organizationId = 0) = 0;

        virtual std::vector<PowerPointDto>
        getPowerHistory(
            int hours = 24,
            int organizationId = 0) = 0;

        virtual std::vector<DeviceDto>
        getDevices(
            int organizationId = 0) = 0;
        
        virtual std::vector<BuildingDto>
        getBuildings(
            int organizationId = 0) = 0;

        virtual std::vector<RoomDto>
        getRooms(
            int organizationId = 0) = 0;

        virtual std::vector<TopConsumerDto>
        getTopConsumers(
            int limit = 10,
            int organizationId = 0) = 0;

        virtual std::vector<AnomalyStatsDto>
        getAnomalyStatistics(
            int organizationId = 0) = 0;

        virtual std::vector<EnergyByRoomDto>
        getEnergyByRoom(
            int organizationId = 0) = 0;

        virtual std::vector<SeverityStatsDto>
        getSeverityDistribution(
            int organizationId = 0) = 0;

        virtual std::optional<UserDto>
        findUserByEmail(
            const std::string &email) = 0;

        virtual std::optional<UserDto>
        findUserById(
            int userId) = 0;

        virtual int
        createUser(
            const UserDto &user) = 0;

        virtual std::optional<UserDto>
        getUserById(
            int userId) = 0;

        virtual bool
        updateUserTelegramChatId(
            int userId,
            const std::string &telegramChatId) = 0;

        virtual Json::Value
        getUserSubscription(
            int userId) = 0;

        virtual Json::Value
        getAdminOverview() = 0;

        virtual int
        createBuildingForOrganization(
            int organizationId,
            const std::string &name,
            const std::string &address,
            const std::string &description) = 0;

        virtual int
        createRoomForBuilding(
            int buildingId,
            const std::string &roomName,
            const std::string &roomType,
            int floorNumber) = 0;

        virtual bool
        createDeviceForRoom(
            const std::string &deviceId,
            const std::string &deviceName,
            const std::string &deviceModel,
            const std::string &firmwareVersion,
            int roomId) = 0;
        // virtual void shutdown() = 0;
    };
} // namespace dorm_energy::storage
