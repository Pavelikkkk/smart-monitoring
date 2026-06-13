#include "dorm_energy/infrastructure/simulation/csv_exporter.hpp"

#include <fstream>
#include <iomanip>
#include <filesystem>

namespace dorm_energy::simulation
{

    bool CsvExporter::exportReadings(
        const core::ReadingsBatch &readings,
        const std::filesystem::path &filePath)
    {
        std::filesystem::create_directories(filePath.parent_path());

        std::ofstream file(filePath);

        if (!file.is_open())
            return false;

        file << "timestamp,device_id,sensor_type,value\n";

        for (const auto &reading : readings)
        {
            double value = reading.value;

            if (reading.sensorType == "motion")
            {
                value = reading.boolValue.value_or(false);
            }

            auto tt = std::chrono::system_clock::to_time_t(reading.timestamp);

            std::tm tm{};

#ifdef _WIN32
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif

            file << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "," << reading.deviceId << "," << reading.sensorType << "," << value << "\n";
        }

        return true;
    }

}