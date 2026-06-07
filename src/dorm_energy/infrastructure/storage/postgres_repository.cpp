// src/dorm_energy/infrastructure/storage/postgres_repository.cpp
#include "dorm_energy/infrastructure/storage/postgres_repository.hpp"
#include "dorm_energy/core/alert_severity.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>
#include <iostream>
#include <pqxx/params>
#include <thread>
#include <chrono>

namespace dorm_energy::storage
{

    PostgresMeasurementRepository::PostgresMeasurementRepository(
        const std::string &connectionString,
        std::size_t maxBufferSize)
        : connectionString_(connectionString),
          maxBufferSize_(maxBufferSize)
    {
        connect();
        std::cout << "[PostgresRepository] Successfully connected to TimescaleDB (buffer size = "
                  << maxBufferSize << ")\n";
    }

    PostgresMeasurementRepository::~PostgresMeasurementRepository()
    {
        flush();
        if (connection_ && connection_->is_open())
        {
            std::cout << "[PostgresRepository] Database connection closed\n";
        }
    }

    void PostgresMeasurementRepository::connect()
    {
        connection_ = std::make_unique<pqxx::connection>(connectionString_);
        if (!connection_->is_open())
            throw std::runtime_error("Failed to open connection to TimescaleDB");
    }

    bool PostgresMeasurementRepository::tryReconnect(int maxAttempts)
    {
        for (int attempt = 1; attempt <= maxAttempts; ++attempt)
        {
            try
            {
                std::cout << "[PostgresRepository] Reconnect attempt " << attempt << "/" << maxAttempts << "...\n";
                connect();
                std::cout << "[PostgresRepository] Reconnected successfully\n";
                return true;
            }
            catch (const std::exception &e)
            {
                std::cerr << "[PostgresRepository] Reconnect failed: " << e.what() << std::endl;
                if (attempt < maxAttempts)
                {
                    auto backoff = std::chrono::seconds(1 << (attempt - 1));
                    std::this_thread::sleep_for(backoff);
                }
            }
        }
        return false;
    }

    bool PostgresMeasurementRepository::save(const core::SensorReading &reading)
    {
        return saveBatch({reading}) == 1;
    }

    std::size_t PostgresMeasurementRepository::saveBatch(const core::ReadingsBatch &readings)
    {
        if (readings.empty())
            return 0;

        {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            buffer_.insert(buffer_.end(), readings.begin(), readings.end());
        }

        if (buffer_.size() >= maxBufferSize_)
            flush();

        return readings.size();
    }

    void PostgresMeasurementRepository::flush()
    {
        std::vector<core::SensorReading> toFlush;
        {
            std::lock_guard<std::mutex> lock(bufferMutex_);
            if (buffer_.empty())
                return;
            toFlush = std::move(buffer_);
        }

        if (toFlush.empty())
            return;

        doFlush(toFlush);
    }

    void PostgresMeasurementRepository::doFlush(const std::vector<core::SensorReading> &readings)
    {
        if (readings.empty())
            return;

        try
        {
            if (!connection_ || !connection_->is_open())
            {
                if (!tryReconnect(3))
                    throw std::runtime_error("Cannot reconnect to database");
            }

            pqxx::work txn{*connection_};
            std::size_t saved = 0;
            std::size_t skipped = 0;

            for (const auto &r : readings)
            {
                std::string ts = fmt::format("{:%Y-%m-%d %H:%M:%S%z}", r.timestamp);

                std::optional<bool> boolVal = r.boolValue;

                try
                {
                    txn.exec(
                        R"(INSERT INTO sensor_readings 
                        (recorded_at, device_id, sensor_type, numeric_value, bool_value, unit)
                       VALUES ($1::timestamptz, $2, $3, $4, $5, $6)
                       ON CONFLICT (recorded_at, device_id, sensor_type) 
                       DO NOTHING)",
                        pqxx::params{ts, r.deviceId, r.sensorType, r.value, boolVal, r.unit});

                    ++saved;
                }
                catch (const pqxx::sql_error &e)
                {
                    ++skipped;
                    std::cerr << "[Postgres] Skipped one reading due to: " << e.what() << std::endl;
                }
            }

            txn.commit();

            if (saved > 0)
                std::cout << fmt::format("[Postgres] Flushed {} readings (skipped {} duplicates)\n", saved, skipped);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Postgres] Critical flush error: " << e.what() << std::endl;

            std::lock_guard<std::mutex> lock(bufferMutex_);
            buffer_.insert(buffer_.begin(), readings.begin(), readings.end());
        }
    }

    bool PostgresMeasurementRepository::saveAnomaly(
        const core::SensorReading &reading,
        const std::string &anomalyType,
        core::AlertSeverity severity,
        const std::string &description,
        double score)
    {
        try
        {
            if (!connection_ || !connection_->is_open())
            {
                if (!tryReconnect(3))
                    return false;
            }

            pqxx::work txn{*connection_};

            std::string ts = fmt::format("{:%Y-%m-%d %H:%M:%S%z}", reading.timestamp);
            std::optional<bool> boolVal = reading.boolValue;

            txn.exec(
                R"(INSERT INTO anomalies 
            (recorded_at, device_id, sensor_type, numeric_value, bool_value, unit,
             anomaly_type, severity, description, score)
           VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10))",
                pqxx::params{
                    ts,
                    reading.deviceId,
                    reading.sensorType,
                    reading.value,
                    boolVal,
                    reading.unit,
                    anomalyType,
                    core::toString(severity),
                    description,
                    score});

            txn.commit();

            std::cout << fmt::format("[Postgres] Anomaly saved: {} - {} ({})\n",
                                     anomalyType,
                                     reading.deviceId,
                                     core::toString(severity));

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Postgres] Save anomaly error: " << e.what() << std::endl;
            return false;
        }
    }
    std::vector<storage::AnomalyDto>
    PostgresMeasurementRepository::getLatestAnomalies(
        std::size_t limit)
    {
        std::vector<AnomalyDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                device_id,
                anomaly_type,
                severity,
                score,
                description,
                detected_at
            FROM anomalies
            ORDER BY detected_at DESC
            LIMIT $1
            )",
                limit);

        for (auto const &row : rows)
        {
            AnomalyDto dto;

            dto.room =
                row["device_id"].c_str();

            dto.type =
                row["anomaly_type"].c_str();

            dto.severity =
                row["severity"].c_str();

            dto.score =
                row["score"].as<double>(0.0);

            dto.description =
                row["description"].c_str();

            dto.detectedAt =
                row["detected_at"].c_str();

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::PowerPointDto>
    PostgresMeasurementRepository::getPowerHistory(
        int hours)
    {
        std::vector<PowerPointDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                time_bucket('1 hour', recorded_at) AS bucket,
                AVG(numeric_value) AS avg_power
            FROM sensor_readings
            WHERE sensor_type = 'power'
              AND recorded_at >= NOW() - ($1 || ' hours')::interval
            GROUP BY bucket
            ORDER BY bucket
            )",
                hours);

        for (auto const &row : rows)
        {
            PowerPointDto dto;

            dto.time =
                row["bucket"].c_str();

            dto.power =
                row["avg_power"].as<double>(0.0);

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::DeviceDto>
    PostgresMeasurementRepository::getDevices()
    {
        std::vector<DeviceDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec(
                R"(
            SELECT
                d.device_id,
                d.device_name,
                COALESCE(d.device_model, '') AS device_model,
                COALESCE(d.firmware_version, '') AS firmware_version,
                COALESCE(r.room_name, '') AS room_name,
                d.is_online,
                COALESCE(
                    d.last_seen_at::text,
                    ''
                ) AS last_seen_at
            FROM devices d
            LEFT JOIN rooms r
                ON r.id = d.room_id
            ORDER BY d.device_name
            )");

        for (auto const &row : rows)
        {
            DeviceDto dto;

            dto.deviceId =
                row["device_id"].c_str();

            dto.deviceName =
                row["device_name"].c_str();

            dto.deviceModel =
                row["device_model"].c_str();

            dto.firmwareVersion =
                row["firmware_version"].c_str();

            dto.roomName =
                row["room_name"].c_str();

            dto.isOnline =
                row["is_online"].as<bool>(false);

            dto.lastSeenAt =
                row["last_seen_at"].c_str();

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::BuildingDto>
    PostgresMeasurementRepository::getBuildings()
    {
        std::vector<BuildingDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec(
                R"(
            SELECT
                id,
                name,
                COALESCE(address, '') AS address,
                COALESCE(description, '') AS description
            FROM buildings
            ORDER BY name
            )");

        for (auto const &row : rows)
        {
            BuildingDto dto;

            dto.id =
                row["id"].as<int>();

            dto.name =
                row["name"].c_str();

            dto.address =
                row["address"].c_str();

            dto.description =
                row["description"].c_str();

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::RoomDto>
    PostgresMeasurementRepository::getRooms()
    {
        std::vector<RoomDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec(
                R"(
            SELECT
                id,
                building_id,
                room_name,
                room_type,
                floor_number
            FROM rooms
            ORDER BY room_name
            )");

        for (const auto &row : rows)
        {
            RoomDto dto;

            dto.id =
                row["id"].as<int>();

            dto.buildingId =
                row["building_id"].as<int>();

            dto.roomName =
                row["room_name"].c_str();

            dto.roomType =
                row["room_type"].c_str();

            dto.floorNumber =
                row["floor_number"].as<int>();

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::TopConsumerDto>
    PostgresMeasurementRepository::getTopConsumers(
        int limit)
    {
        std::vector<TopConsumerDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                device_id,
                AVG(numeric_value) AS avg_power
            FROM sensor_readings
            WHERE sensor_type = 'power'
            GROUP BY device_id
            ORDER BY avg_power DESC
            LIMIT $1
            )",
                limit);

        for (const auto &row : rows)
        {
            TopConsumerDto dto;

            dto.roomName =
                row["device_id"].c_str();

            dto.power =
                row["avg_power"].as<double>(0.0);

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::AnomalyStatsDto>
    PostgresMeasurementRepository::getAnomalyStatistics()
    {
        std::vector<AnomalyStatsDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec(
                R"(
            SELECT
                anomaly_type,
                COUNT(*) AS total
            FROM anomalies
            GROUP BY anomaly_type
            ORDER BY total DESC
            )");

        for (const auto &row : rows)
        {
            AnomalyStatsDto dto;

            dto.type =
                row["anomaly_type"].c_str();

            dto.count =
                row["total"].as<int>();

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::EnergyByRoomDto>
    PostgresMeasurementRepository::getEnergyByRoom()
    {
        std::vector<EnergyByRoomDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec(
                R"(
            SELECT
                device_id,
                AVG(numeric_value) AS avg_power
            FROM sensor_readings
            WHERE sensor_type = 'power'
            GROUP BY device_id
            ORDER BY avg_power DESC
            )");

        for (auto const &row : rows)
        {
            EnergyByRoomDto dto;

            dto.roomName =
                row["device_id"].c_str();

            dto.avgPower =
                row["avg_power"].as<double>(0.0);

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::SeverityStatsDto>
    PostgresMeasurementRepository::getSeverityDistribution()
    {
        std::vector<SeverityStatsDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec(
                R"(
            SELECT
                severity,
                COUNT(*) AS total
            FROM anomalies
            GROUP BY severity
            ORDER BY total DESC
            )");

        for (auto const &row : rows)
        {
            SeverityStatsDto dto;

            dto.severity =
                row["severity"].c_str();

            dto.count =
                row["total"].as<int>(0);

            result.push_back(dto);
        }

        return result;
    }
} // namespace dorm_energy::storage