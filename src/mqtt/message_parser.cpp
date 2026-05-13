// src/mqtt/message_parser.cpp
#include <nlohmann/json.hpp>
#include <format>
#include <chrono>

#include "dorm_energy/mqtt/message_parser.hpp"

namespace dorm_energy::mqtt
{

    bool MessageParser::parse(const std::string &payload, core::PowerMeasurement &out)
    {
        try
        {
            auto j = nlohmann::json::parse(payload);

            out.timestamp = std::chrono::system_clock::now();
            out.hour_of_day = j.value("hour", 0);
            out.power_kw = j.value("power_kw", 0.0);
            out.is_anomaly = false;

            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    std::string MessageParser::to_json(const core::PowerMeasurement &m)
    {
        nlohmann::json j;
        j["power_kw"] = m.power_kw;
        j["hour"] = m.hour_of_day;
        j["timestamp"] = std::format("{:%Y-%m-%d %H:%M}", m.timestamp);
        return j.dump();
    }

} // namespace dorm_energy::mqtt