#include "dorm_energy/infrastructure/logging/spdlog_logger.hpp"
#include "dorm_energy/application/config/app_config.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/common.h>

namespace dorm_energy::logging
{

    spdlog::level::level_enum SpdlogLogger::toSpdlogLevel(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return spdlog::level::debug;
        case LogLevel::Info:
            return spdlog::level::info;
        case LogLevel::Warn:
            return spdlog::level::warn;
        case LogLevel::Error:
            return spdlog::level::err;
        case LogLevel::Critical:
            return spdlog::level::critical;
        default:
            return spdlog::level::info;
        }
    }

    SpdlogLogger::SpdlogLogger(const std::string &name) : logger_(spdlog::stdout_color_mt(name))
    {
        logger_->set_level(spdlog::level::info);
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    }

    SpdlogLogger::SpdlogLogger(const application::AppConfig &config,
                               const std::string &name) : logger_(spdlog::stdout_color_mt(name))
    {
        spdlog::level::level_enum spdLevel = spdlog::level::info;

        const std::string &levelStr = config.getLogLevel();

        if (levelStr == "debug")
            spdLevel = spdlog::level::debug;
        else if (levelStr == "info")
            spdLevel = spdlog::level::info;
        else if (levelStr == "warn")
            spdLevel = spdlog::level::warn;
        else if (levelStr == "error")
            spdLevel = spdlog::level::err;
        else if (levelStr == "critical")
            spdLevel = spdlog::level::critical;

        logger_->set_level(spdLevel);
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        if (config.isVerbose())
        {
            logger_->info("SpdlogLogger initialized in verbose mode");
        }
    }

    void SpdlogLogger::log(LogLevel level, const std::string &message)
    {
        logger_->log(toSpdlogLevel(level), message);
    }

} // namespace dorm_energy::logging