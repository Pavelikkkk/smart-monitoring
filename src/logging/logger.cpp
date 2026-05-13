// src/logging/logger.cpp
#include "dorm_energy/logging/logger.hpp"

namespace dorm_energy::logging
{

    Logger::Logger(std::string name)
        : name_(std::move(name))
    {
    }

    void Logger::log(const std::string &level, const std::string &message) const
    {
        auto now = std::chrono::system_clock::now();
        std::cout << std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] [{}] {}\n",
                                 now, name_, level, message);
    }

    void Logger::info(const std::string &message) const { log("INFO", message); }
    void Logger::debug(const std::string &message) const { log("DEBUG", message); }
    void Logger::warn(const std::string &message) const { log("WARN", message); }
    void Logger::error(const std::string &message) const { log("ERROR", message); }

} // namespace dorm_energy::logging