// include/dorm_energy/infrastructure/logging/spdlog_logger.hpp
#pragma once

#include "dorm_energy/application/config/app_config.hpp"
#include "dorm_energy/domain/logging/ilogger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace dorm_energy::logging
{

    class SpdlogLogger : public dorm_energy::logging::ILogger
    {
    public:
        explicit SpdlogLogger(const std::string &name = "dorm-energy");

        explicit SpdlogLogger(const dorm_energy::application::AppConfig &config,
                              const std::string &name = "dorm-energy");

        void log(dorm_energy::logging::LogLevel level, const std::string &message) override;

    private:
        std::shared_ptr<spdlog::logger> logger_;
        static spdlog::level::level_enum toSpdlogLevel(dorm_energy::logging::LogLevel level);
    };

} // namespace dorm_energy::logging