#pragma once

#include "../interfaces.hpp"
#include "../core/measurement.hpp"
#include <memory>

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
            StorageConfig config = {},
            std::unique_ptr<ILogger> logger = nullptr);

        ~MeasurementRepository() override;

        void save(const core::PowerMeasurement &measurement) override;
        void save_batch(const core::SimulationData &data) override;

    private:
        StorageConfig config_;
        std::unique_ptr<ILogger> logger_;
        bool connected_ = false;
    };

} // namespace dorm_energy::storage