#pragma once

#include "../core/measurement.hpp"
#include "../interfaces.hpp"

namespace dorm_energy::storage
{
    struct StorageConfig 
    {
        std::string host = "localhost";
        std::string port = "5432";
        std::string dbname = "dorm_energy";
        std::string user = "postgres";
        std::string password = "password";
    };

    class MeasurementRepository : public IMeasurementRepository
    {
    public:
        explicit MeasurementRepository(
            StorageConfig config = StorageConfig{});
        ~MeasurementRepository() override;

        bool connect();
        void save(const core::PowerMeasurement &measurement) override;
        void save_batch(const core::SimulationData &data) override;

        // получение данных позже
        // std::vector<core::PowerMeasurement> get_last_hour() const;

    private:
        StorageConfig config_;
        // void* connection_ = nullptr; // pqxx::connection* позже
        bool connected_ = false;
    };

} // namespace dorm_energy::storage