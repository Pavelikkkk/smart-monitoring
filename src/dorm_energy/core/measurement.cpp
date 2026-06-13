#include "dorm_energy/core/measurement.hpp"

#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime> 
#include <string>

namespace dorm_energy::core
{

    int getHourOfDay(const SensorReading &reading)
    {
        if (reading.timestamp == std::chrono::system_clock::time_point{})
        {
            return 0;
        }

        std::time_t time_t = std::chrono::system_clock::to_time_t(reading.timestamp);
        std::tm *tm = std::localtime(&time_t);

        return tm ? tm->tm_hour : 0; 
    }

    std::string toString(const SensorReading &reading)
    {
        std::ostringstream oss;

        if (reading.timestamp != std::chrono::system_clock::time_point{})
        {
            std::time_t time_t = std::chrono::system_clock::to_time_t(reading.timestamp);
            std::tm *tm = std::localtime(&time_t);
            if (tm)
            {
                oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
            }
        }
        else
        {
            oss << "0000-00-00 00:00:00";
        }

        oss << " | " << reading.deviceId << " | " << reading.sensorType;

        if (reading.boolValue.has_value())
        {
            oss << " | " << (reading.boolValue.value() ? "true" : "false");
        }
        else
        {
            oss << " | " << reading.value;
            if (!reading.unit.empty())
            {
                oss << " " << reading.unit;
            }
        }

        return oss.str();
    }

    std::string toJson(const SensorReading &reading)
    {
        std::ostringstream oss;
        oss << "{";

        oss << "\"timestamp\":";
        if (reading.timestamp != std::chrono::system_clock::time_point{})
        {
            std::time_t time_t = std::chrono::system_clock::to_time_t(reading.timestamp);
            std::tm *tm = std::gmtime(&time_t); 
            if (tm)
            {
                oss << "\"" << std::put_time(tm, "%Y-%m-%dT%H:%M:%SZ") << "\"";
            }
            else
            {
                oss << "null";
            }
        }
        else
        {
            oss << "null";
        }

        oss << ",\"deviceId\":\"" << reading.deviceId << "\"" << ",\"sensorType\":\"" << reading.sensorType << "\"";

        if (reading.boolValue.has_value())
        {
            oss << ",\"value\":" << (reading.boolValue.value() ? "true" : "false") 
            << ",\"boolValue\":" << (reading.boolValue.value() ? "true" : "false");
        }
        else
        {
            oss << ",\"value\":" << reading.value;
        }

        if (!reading.unit.empty())
        {
            oss << ",\"unit\":\"" << reading.unit << "\"";
        }

        oss << "}";
        return oss.str();
    }

} // namespace dorm_energy::core