#include "dorm_energy/infrastructure/storage/postgres_repository.hpp"
#include "dorm_energy/core/alert_severity.hpp"
#include "dorm_energy/domain/storage/user_dto.hpp"

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
        std::cout << "[PostgresRepository] Successfully connected to TimescaleDB (buffer size = " << maxBufferSize << ")\n";
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
                                     anomalyType, reading.deviceId, core::toString(severity));

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Postgres] Save anomaly error: " << e.what() << std::endl;
            return false;
        }
    }
    std::vector<storage::AnomalyDto> PostgresMeasurementRepository::getLatestAnomalies(
        std::size_t limit,
        int organizationId)
    {
        std::vector<AnomalyDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                anomalies.device_id,
                anomalies.anomaly_type,
                anomalies.severity,
                anomalies.score,
                anomalies.description,
                anomalies.detected_at,
                r.room_name
            FROM anomalies
            JOIN devices d
                ON d.device_id = anomalies.device_id
            JOIN rooms r
                ON r.id = d.room_id
            JOIN buildings b
                ON b.id = r.building_id
            WHERE ($2 = 0 OR b.organization_id = $2)
            ORDER BY detected_at DESC
            LIMIT $1
            )",
                limit,
                organizationId);

        for (auto const &row : rows)
        {
            AnomalyDto dto;

            dto.room = row["room_name"].c_str();
            dto.type = row["anomaly_type"].c_str();
            dto.severity = row["severity"].c_str();
            dto.score = row["score"].as<double>(0.0);
            dto.description = row["description"].c_str();
            dto.detectedAt = row["detected_at"].c_str();

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::PowerPointDto> PostgresMeasurementRepository::getPowerHistory(
        int hours,
        int organizationId)
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
            JOIN devices d
                ON d.device_id = sensor_readings.device_id
            JOIN rooms r
                ON r.id = d.room_id
            JOIN buildings b
                ON b.id = r.building_id
            WHERE sensor_type = 'power'
              AND recorded_at >= NOW() - ($1 || ' hours')::interval
              AND ($2 = 0 OR b.organization_id = $2)
            GROUP BY bucket
            ORDER BY bucket
            )",
                hours,
                organizationId);

        for (auto const &row : rows)
        {
            PowerPointDto dto;

            dto.time = row["bucket"].c_str();
            dto.power = row["avg_power"].as<double>(0.0);

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::DeviceDto> PostgresMeasurementRepository::getDevices(
        int organizationId)
    {
        std::vector<DeviceDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
            d.device_id,
            d.device_name,
            d.device_model,
            d.firmware_version,

            r.id AS room_id,
            r.room_name,

            b.id AS building_id,

            o.id AS organization_id,

            d.is_online,

            COALESCE(
                d.last_seen_at::text,
                ''
            ) AS last_seen_at

        FROM devices d

        LEFT JOIN rooms r
            ON r.id = d.room_id

        LEFT JOIN buildings b
            ON b.id = r.building_id

        LEFT JOIN organizations o
            ON o.id = b.organization_id
        WHERE ($1 = 0 OR b.organization_id = $1)
            )",
                organizationId);

        for (auto const &row : rows)
        {
            DeviceDto dto;

            dto.deviceId = row["device_id"].c_str();
            dto.deviceName = row["device_name"].c_str();
            dto.deviceModel = row["device_model"].c_str();
            dto.firmwareVersion = row["firmware_version"].c_str();
            dto.roomName = row["room_name"].c_str();
            dto.roomId = row["room_id"].as<int>(0);
            dto.buildingId = row["building_id"].as<int>(0);
            dto.organizationId = row["organization_id"].as<int>(0);
            dto.isOnline = row["is_online"].as<bool>(false);
            dto.lastSeenAt = row["last_seen_at"].c_str();

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::BuildingDto> PostgresMeasurementRepository::getBuildings(
        int organizationId)
    {
        std::vector<BuildingDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                id,
                name,
                COALESCE(address, '') AS address,
                COALESCE(description, '') AS description
            FROM buildings
            WHERE ($1 = 0 OR organization_id = $1)
            ORDER BY name
            )",
                organizationId);

        for (auto const &row : rows)
        {
            BuildingDto dto;

            dto.id = row["id"].as<int>();
            dto.name = row["name"].c_str();
            dto.address = row["address"].c_str();
            dto.description = row["description"].c_str();

            result.push_back(dto);
        }

        return result;
    }

    std::vector<storage::RoomDto> PostgresMeasurementRepository::getRooms(
        int organizationId)
    {
        std::vector<RoomDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                r.id,
                r.building_id,
                r.room_name,
                r.room_type,
                r.floor_number
            FROM rooms r
            JOIN buildings b
                ON b.id = r.building_id
            WHERE ($1 = 0 OR b.organization_id = $1)
            ORDER BY room_name
            )",
                organizationId);

        for (const auto &row : rows)
        {
            RoomDto dto;

            dto.id = row["id"].as<int>();
            dto.buildingId = row["building_id"].as<int>();
            dto.roomName = row["room_name"].c_str();
            dto.roomType = row["room_type"].c_str();
            dto.floorNumber = row["floor_number"].as<int>();

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::TopConsumerDto> PostgresMeasurementRepository::getTopConsumers(
        int limit,
        int organizationId)
    {
        std::vector<TopConsumerDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                sensor_readings.device_id,
                AVG(numeric_value) AS avg_power
            FROM sensor_readings
            JOIN devices d
                ON d.device_id = sensor_readings.device_id
            JOIN rooms r
                ON r.id = d.room_id
            JOIN buildings b
                ON b.id = r.building_id
            WHERE sensor_type = 'power'
              AND ($2 = 0 OR b.organization_id = $2)
            GROUP BY sensor_readings.device_id
            ORDER BY avg_power DESC
            LIMIT $1
            )",
                limit,
                organizationId);

        for (const auto &row : rows)
        {
            TopConsumerDto dto;

            dto.roomName = row["device_id"].c_str();
            dto.power = row["avg_power"].as<double>(0.0);

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::AnomalyStatsDto> PostgresMeasurementRepository::getAnomalyStatistics(
        int organizationId)
    {
        std::vector<AnomalyStatsDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                anomaly_type,
                COUNT(*) AS total
            FROM anomalies
            JOIN devices d
                ON d.device_id = anomalies.device_id
            JOIN rooms r
                ON r.id = d.room_id
            JOIN buildings b
                ON b.id = r.building_id
            WHERE ($1 = 0 OR b.organization_id = $1)
            GROUP BY anomaly_type
            ORDER BY total DESC
            )",
                organizationId);

        for (const auto &row : rows)
        {
            AnomalyStatsDto dto;

            dto.type = row["anomaly_type"].c_str();
            dto.count = row["total"].as<int>();

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::EnergyByRoomDto> PostgresMeasurementRepository::getEnergyByRoom(
        int organizationId)
    {
        std::vector<EnergyByRoomDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                r.room_name,
                AVG(numeric_value) AS avg_power
            FROM sensor_readings
            JOIN devices d
                ON d.device_id = sensor_readings.device_id
            JOIN rooms r
                ON r.id = d.room_id
            JOIN buildings b
                ON b.id = r.building_id
            WHERE sensor_type = 'power'
              AND ($1 = 0 OR b.organization_id = $1)
            GROUP BY r.room_name
            ORDER BY avg_power DESC
            )",
                organizationId);

        for (auto const &row : rows)
        {
            EnergyByRoomDto dto;

            dto.roomName = row["room_name"].c_str();
            dto.avgPower = row["avg_power"].as<double>(0.0);

            result.push_back(dto);
        }

        return result;
    }
    std::vector<storage::SeverityStatsDto> PostgresMeasurementRepository::getSeverityDistribution(
        int organizationId)
    {
        std::vector<SeverityStatsDto> result;

        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                severity,
                COUNT(*) AS total
            FROM anomalies
            JOIN devices d
                ON d.device_id = anomalies.device_id
            JOIN rooms r
                ON r.id = d.room_id
            JOIN buildings b
                ON b.id = r.building_id
            WHERE ($1 = 0 OR b.organization_id = $1)
            GROUP BY severity
            ORDER BY total DESC
            )",
                organizationId);

        for (auto const &row : rows)
        {
            SeverityStatsDto dto;

            dto.severity = row["severity"].c_str();
            dto.count = row["total"].as<int>(0);

            result.push_back(dto);
        }

        return result;
    }

    std::optional<UserDto> PostgresMeasurementRepository::findUserByEmail(
        const std::string &email)
    {
        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                id,
                username,
                email,
                password_hash,
                role,
                organization_id,
                account_type,
                COALESCE(telegram_chat_id, '') AS telegram_chat_id
            FROM users
            WHERE email = $1
            )",
                email);

        if (rows.empty())
        {
            return std::nullopt;
        }

        const auto &row = rows[0];

        UserDto dto;

        dto.id = row["id"].as<int>();
        dto.username = row["username"].c_str();
        dto.email = row["email"].c_str();
        dto.passwordHash = row["password_hash"].c_str();
        dto.role = row["role"].c_str();
        dto.organizationId = row["organization_id"].is_null() ? 0 : row["organization_id"].as<int>();
        dto.accountType = row["account_type"].c_str();
        dto.telegramChatId = row["telegram_chat_id"].c_str();

        return dto;
    }

    std::optional<UserDto> PostgresMeasurementRepository::findUserById(
        int userId)
    {
        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                id,
                username,
                email,
                password_hash,
                role,
                organization_id,
                account_type,
                COALESCE(telegram_chat_id, '') AS telegram_chat_id
            FROM users
            WHERE id = $1
            )",
                userId);

        if (rows.empty())
        {
            return std::nullopt;
        }

        const auto &row = rows[0];

        UserDto dto;

        dto.id = row["id"].as<int>();
        dto.username = row["username"].c_str();
        dto.email = row["email"].c_str();
        dto.passwordHash = row["password_hash"].c_str();
        dto.role = row["role"].c_str();
        dto.organizationId = row["organization_id"].is_null() ? 0 : row["organization_id"].as<int>();
        dto.accountType = row["account_type"].c_str();
        dto.telegramChatId = row["telegram_chat_id"].c_str();

        return dto;
    }

    int PostgresMeasurementRepository::createUser(
        const UserDto &user)
    {
        try
        {
            pqxx::work txn(*connection_);

            const auto organizationType = user.accountType == "BUSINESS" ? "BUSINESS" : "PERSONAL";

            auto organizationRow =
                txn.exec_params1(
                    R"(
        INSERT INTO organizations
        (
            name,
            type
        )
        VALUES
        (
            $1,
            $2
        )
        RETURNING id
        )",
                    user.username + " workspace",
                    organizationType);

            const auto organizationId = organizationRow["id"].as<int>();

            auto row = txn.exec_params1(
                R"(
        INSERT INTO users
        (
            username,
            email,
            password_hash,
            role,
            account_type,
            organization_id
        )
        VALUES
        (
            $1,
            $2,
            $3,
            $4,
            $5,
            $6
        )
        RETURNING id
        )",
                user.username,
                user.email,
                user.passwordHash,
                user.role,
                user.accountType,
                organizationId);

            const auto userId = row["id"].as<int>();

            const auto isBusiness = user.accountType == "BUSINESS";

            txn.exec_params(
                R"(
        INSERT INTO subscriptions
        (
            organization_id,
            user_id,
            plan,
            status,
            max_buildings,
            max_rooms,
            max_devices
        )
        VALUES
        (
            $1,
            $2,
            $3,
            'ACTIVE',
            $4,
            $5,
            $6
        )
        )",
                organizationId,
                userId,
                isBusiness ? "BUSINESS" : "STANDARD",
                isBusiness ? 25 : 0,
                isBusiness ? 250 : 5,
                isBusiness ? 1200 : 20);

            txn.commit();

            return userId;
        }
        catch (const std::exception &ex)
        {
            std::cerr << "CREATE USER ERROR: " << ex.what() << std::endl;

            throw;
        }
    }

    std::optional<UserDto> PostgresMeasurementRepository::getUserById(
        int userId)
    {
        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                id,
                username,
                email,
                password_hash,
                role,
                organization_id,
                account_type,
                COALESCE(telegram_chat_id, '') AS telegram_chat_id
            FROM users
            WHERE id = $1
            )",
                userId);

        if (rows.empty())
        {
            return std::nullopt;
        }

        UserDto dto;

        dto.id = rows[0]["id"].as<int>();

        dto.username = rows[0]["username"].c_str();

        dto.email = rows[0]["email"].c_str();

        dto.passwordHash = rows[0]["password_hash"].c_str();

        dto.role = rows[0]["role"].c_str();

        dto.organizationId = rows[0]["organization_id"].as<int>(0);

        dto.accountType = rows[0]["account_type"].c_str();

        dto.telegramChatId = rows[0]["telegram_chat_id"].c_str();

        return dto;
    }

    bool PostgresMeasurementRepository::updateUserTelegramChatId(
        int userId,
        const std::string &telegramChatId)
    {
        pqxx::work txn(*connection_);

        auto result =
            txn.exec_params(
                R"(
            UPDATE users
            SET telegram_chat_id = $2,
                updated_at = NOW()
            WHERE id = $1
            )",
                userId,
                telegramChatId);

        txn.commit();

        return result.affected_rows() == 1;
    }

    Json::Value PostgresMeasurementRepository::getUserSubscription(
        int userId)
    {
        pqxx::work txn(*connection_);

        auto rows =
            txn.exec_params(
                R"(
            SELECT
                plan,
                status,
                max_buildings,
                max_rooms,
                max_devices
            FROM subscriptions
            WHERE user_id = $1
            ORDER BY created_at DESC
            LIMIT 1
            )",
                userId);

        Json::Value json;

        if (rows.empty())
        {
            json["plan"] = "STANDARD";
            json["status"] = "ACTIVE";
            json["maxBuildings"] = 0;
            json["maxRooms"] = 5;
            json["maxDevices"] = 20;
            return json;
        }

        const auto &row = rows[0];

        json["plan"] = row["plan"].c_str();
        json["status"] = row["status"].c_str();
        json["maxBuildings"] = row["max_buildings"].as<int>(0);
        json["maxRooms"] = row["max_rooms"].as<int>(0);
        json["maxDevices"] = row["max_devices"].as<int>(0);

        return json;
    }

    Json::Value PostgresMeasurementRepository::getAdminOverview()
    {
        pqxx::work txn(*connection_);

        Json::Value json;
        json["users"] = Json::Value(Json::arrayValue);
        json["buildings"] = Json::Value(Json::arrayValue);
        json["rooms"] = Json::Value(Json::arrayValue);

        auto users =
            txn.exec(
                R"(
            SELECT
                u.id,
                u.username,
                u.email,
                u.role,
                COALESCE(u.account_type, 'PERSONAL') AS account_type,
                COALESCE(u.organization_id, 0) AS organization_id,
                COALESCE(o.name, '') AS organization_name,
                COALESCE(s.plan, 'STANDARD') AS plan,
                COUNT(DISTINCT b.id) AS buildings_count,
                COUNT(DISTINCT r.id) AS rooms_count,
                COUNT(DISTINCT d.device_id) AS devices_count
            FROM users u
            LEFT JOIN organizations o
                ON o.id = u.organization_id
            LEFT JOIN subscriptions s
                ON s.user_id = u.id
            LEFT JOIN buildings b
                ON b.organization_id = u.organization_id
            LEFT JOIN rooms r
                ON r.building_id = b.id
            LEFT JOIN devices d
                ON d.room_id = r.id
            GROUP BY
                u.id,
                u.username,
                u.email,
                u.role,
                u.account_type,
                u.organization_id,
                o.name,
                s.plan
            ORDER BY u.id
            )");

        for (const auto &row : users)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["username"] = row["username"].c_str();
            item["email"] = row["email"].c_str();
            item["role"] = row["role"].c_str();
            item["accountType"] = row["account_type"].c_str();
            item["organizationId"] = row["organization_id"].as<int>(0);
            item["organizationName"] = row["organization_name"].c_str();
            item["plan"] = row["plan"].c_str();
            item["buildingsCount"] = row["buildings_count"].as<int>(0);
            item["roomsCount"] = row["rooms_count"].as<int>(0);
            item["devicesCount"] = row["devices_count"].as<int>(0);

            json["users"].append(item);
        }

        auto buildings =
            txn.exec(
                R"(
            SELECT
                id,
                organization_id,
                name,
                COALESCE(address, '') AS address
            FROM buildings
            ORDER BY organization_id, name
            )");

        for (const auto &row : buildings)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["organizationId"] = row["organization_id"].as<int>(0);
            item["name"] = row["name"].c_str();
            item["address"] = row["address"].c_str();

            json["buildings"].append(item);
        }

        auto rooms =
            txn.exec(
                R"(
            SELECT
                id,
                building_id,
                room_name,
                room_type,
                floor_number
            FROM rooms
            ORDER BY building_id, room_name
            )");

        for (const auto &row : rooms)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["buildingId"] = row["building_id"].as<int>();
            item["roomName"] = row["room_name"].c_str();
            item["roomType"] = row["room_type"].c_str();
            item["floorNumber"] = row["floor_number"].as<int>();

            json["rooms"].append(item);
        }

        return json;
    }

    int PostgresMeasurementRepository::createBuildingForOrganization(
        int organizationId,
        const std::string &name,
        const std::string &address,
        const std::string &description)
    {
        pqxx::work txn(*connection_);

        auto row =
            txn.exec_params1(
                R"(
            INSERT INTO buildings
            (
                organization_id,
                name,
                address,
                description
            )
            VALUES
            (
                $1,
                $2,
                $3,
                $4
            )
            RETURNING id
            )",
                organizationId,
                name,
                address,
                description);

        txn.commit();

        return row["id"].as<int>();
    }

    int PostgresMeasurementRepository::createRoomForBuilding(
        int buildingId,
        const std::string &roomName,
        const std::string &roomType,
        int floorNumber)
    {
        pqxx::work txn(*connection_);

        auto row =
            txn.exec_params1(
                R"(
            INSERT INTO rooms
            (
                building_id,
                room_name,
                room_type,
                floor_number
            )
            VALUES
            (
                $1,
                $2,
                $3,
                $4
            )
            RETURNING id
            )",
                buildingId,
                roomName,
                roomType,
                floorNumber);

        txn.commit();

        return row["id"].as<int>();
    }

    bool PostgresMeasurementRepository::createDeviceForRoom(
        const std::string &deviceId,
        const std::string &deviceName,
        const std::string &deviceModel,
        const std::string &firmwareVersion,
        int roomId)
    {
        pqxx::work txn(*connection_);

        txn.exec_params(
            R"(
            INSERT INTO devices
            (
                device_id,
                device_name,
                device_model,
                firmware_version,
                room_id,
                is_online,
                last_seen_at
            )
            VALUES
            (
                $1,
                $2,
                $3,
                $4,
                $5,
                TRUE,
                NOW()
            )
            ON CONFLICT (device_id)
            DO UPDATE SET
                device_name = EXCLUDED.device_name,
                device_model = EXCLUDED.device_model,
                firmware_version = EXCLUDED.firmware_version,
                room_id = EXCLUDED.room_id,
                is_online = TRUE,
                last_seen_at = NOW()
            )",
            deviceId,
            deviceName,
            deviceModel,
            firmwareVersion,
            roomId);

        txn.commit();

        return true;
    }
} // namespace dorm_energy::storage
