#include "dorm_energy/infrastructure/storage/postgres_repository.hpp"

#include <gtest/gtest.h>
#include <pqxx/pqxx>

#include <chrono>
#include <cstdlib>
#include <string>

namespace
{
    std::string testConnectionString()
    {
        const char *value =
            std::getenv(
                "DORM_ENERGY_TEST_DATABASE_URL");

        if (value == nullptr || std::string(value).empty())
        {
            return {};
        }

        return value;
    }
}

TEST(PostgresRepositoryIntegrationTest, SaveBatchFlushesReadingsToDatabase)
{
    const auto connectionString =
        testConnectionString();

    if (connectionString.empty())
    {
        GTEST_SKIP()
            << "Set DORM_ENERGY_TEST_DATABASE_URL to run PostgreSQL integration tests";
    }

    constexpr const char *deviceId =
        "admin-lobby-power-01";

    const auto timestamp =
        std::chrono::system_clock::now();

    dorm_energy::core::SensorReading reading{
        .timestamp = timestamp,
        .deviceId = deviceId,
        .sensorType = "power",
        .value = 321.5,
        .boolValue = std::nullopt,
        .unit = "W"};

    {
        dorm_energy::storage::PostgresMeasurementRepository repository{
            connectionString,
            1};

        EXPECT_EQ(
            repository.saveBatch(
                {reading}),
            1U);

        repository.flush();
    }

    pqxx::connection connection{
        connectionString};

    pqxx::work transaction{
        connection};

    const auto result =
        transaction.exec_params(
            R"(
                SELECT COUNT(*)
                FROM sensor_readings
                WHERE device_id = $1
                  AND sensor_type = $2
                  AND numeric_value = $3
                  AND unit = $4
            )",
            deviceId,
            "power",
            reading.value,
            reading.unit);

    ASSERT_EQ(
        result.size(),
        1U);

    EXPECT_GE(
        result[0][0].as<int>(),
        1);
}

TEST(PostgresRepositoryIntegrationTest, ReadsSeededBuildingsRoomsAndDevices)
{
    const auto connectionString =
        testConnectionString();

    if (connectionString.empty())
    {
        GTEST_SKIP()
            << "Set DORM_ENERGY_TEST_DATABASE_URL to run PostgreSQL integration tests";
    }

    dorm_energy::storage::PostgresMeasurementRepository repository{
        connectionString,
        1};

    EXPECT_FALSE(
        repository.getBuildings().empty());

    EXPECT_FALSE(
        repository.getRooms().empty());

    EXPECT_FALSE(
        repository.getDevices().empty());
}
